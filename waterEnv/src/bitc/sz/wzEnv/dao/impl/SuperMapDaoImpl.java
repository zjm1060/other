package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.dao.SuperMapDao;
import bitc.sz.wzEnv.exception.NullException;
import bitc.sz.wzEnv.model.IdAndName;
import bitc.sz.wzEnv.model.ShuiZiYuanBody;
import bitc.sz.wzEnv.model.TypeBody;
import bitc.sz.wzEnv.model.WaterBody;
import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.util.DBUtil;
import bitc.sz.wzEnv.util.MapSortUtil;
import bitc.sz.wzEnv.util.Md5Util;
import com.sun.xml.xsom.impl.scd.Iterators;
import oracle.jdbc.OracleTypes;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import javax.sql.DataSource;
import java.io.File;
import java.math.BigDecimal;
import java.sql.*;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by zsz on 2015/12/25.
 */
@Repository
public class SuperMapDaoImpl extends BaseDAO implements SuperMapDao {
    @Autowired
    private DataSource basicDataSource;

    @Override
    public String login(String userName, String password) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call sp_userValidate(?,?,?)}");
            call.setString(1, userName);
            call.setString(2, password);
            call.registerOutParameter(3, OracleTypes.VARCHAR);
            call.execute();
            return call.getString(3);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(call != null)
                call.close();
                if(connection != null)
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<WaterBody> getWaterBodyByUserID(String userId) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call sp_getWaterBodysByUserID(?,?)}");
            call.setString(1, userId);
            call.registerOutParameter(2, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(2);
            List<WaterBody> list = new ArrayList<WaterBody>();
            while (rs.next()) {
                WaterBody waterBody = new WaterBody();
                waterBody.setId(rs.getString(1));
                waterBody.setName(rs.getString(2));
                waterBody.setMapId(rs.getString(3));
                waterBody.setSuperObjId(rs.getString(4));
                waterBody.setIcoFlag(rs.getString(5));
                String typeIds = "";
                for (int i = 6; i <= 9; i++) {
                    if (rs.getString(i) != null && !"".equals(rs.getString(i))) {
                        typeIds += rs.getString(i);
                        typeIds += ",";
                    }
                }
                if (typeIds.length() > 0)
                    waterBody.setTypeIDs(typeIds.substring(0, typeIds.length() - 1));
                list.add(waterBody);
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                rs.close();
                if(call != null)
                call.close();
                if(connection != null)
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<IdAndName> getMainTypes(String userId, String waterBodyId) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call sp_getMainTypes(?,?,?)}");
            call.setString(1, userId);
            call.setString(2, waterBodyId);
            call.registerOutParameter(3, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(3);
            List<IdAndName> list = new ArrayList<IdAndName>();
            while (rs.next()) {
                IdAndName idAndName = new IdAndName();
                idAndName.setId(rs.getString(1));
                idAndName.setName(rs.getString(2));
                list.add(idAndName);
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<TypeBody> getTypes(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            call = connection.prepareCall("{call SP_GETTYPES(?,?,?,?,?,?)}");
            call.setString(1, userId);
            call.setString(2, waterBodyId);
            call.setString(3, mainTypeId);
            call.setString(4, typeIds);
            call.setString(5, keyword);
            call.registerOutParameter(6, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(6);
            List<TypeBody> list = new ArrayList<TypeBody>();
            while (rs.next()) {
                TypeBody typeBody = new TypeBody();
                typeBody.setTypeId(rs.getString(1));
                typeBody.setTypeName(rs.getString(2));
                typeBody.setIcoFlag(rs.getString(3));
                list.add(typeBody);
            }
            connection.commit();
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<ShuiZiYuanBody> getObjsByType(String userId, String waterBodyId, String mainTypeId, String typeIds) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETOBJSBYTYPE(?,?,?,?,?)}");
            call.setString(1, userId);
            call.setString(2, waterBodyId);
            call.setString(3, mainTypeId);
            call.setString(4, typeIds);
            call.registerOutParameter(5, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(5);
            List<ShuiZiYuanBody> list = new ArrayList<ShuiZiYuanBody>();
            while (rs.next()) {
                ShuiZiYuanBody shuiZiYuanBody = new ShuiZiYuanBody();
                shuiZiYuanBody.setId(rs.getString(1));
                shuiZiYuanBody.setName(rs.getString(2));
                shuiZiYuanBody.setLocation(rs.getString(3));
                shuiZiYuanBody.setMapId(rs.getString(4));
                shuiZiYuanBody.setSuperObjId(rs.getString(5));
                shuiZiYuanBody.setIcoFlag(rs.getString(6));
                String str = "";
                for (int i = 7; i <= 10; i++) {
                    if (rs.getString(i) != null && !"".equals(rs.getString(i))) {
                        str += rs.getString(i);
                        str += ",";
                    }
                }
                if (str.length() > 0)
                    shuiZiYuanBody.setTypeIDs(str.substring(0, str.length() - 1));
                list.add(shuiZiYuanBody);
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

//    @Override
//    public List<Map<Object,Object>> getTarget2(String userId, String targetIds,String type ,String water_body_id,String searchType,String period_id) throws SQLException {
//        Connection connection = null;
//        CallableStatement call = null;
//        PreparedStatement ps = null;
//        PreparedStatement ps1 = null;
//        PreparedStatement ps2 = null;
//        ResultSet rs = null;
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//        String startDate = null;
//        String endDate = null;
//        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//        SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//        List list = new ArrayList();
//        //存放某一个断面最新时间的水文观测数据
////        List<Map<Object,Object>> swList = new ArrayList<Map<Object,Object>>();
//        Map<Object,Object> swmap = null;
//        //存放某一个断面最新时间的水质指标观测数据
//        Map<Object,Object> szMap = null;
//        //先得到起止日期
//        if(StringUtils.isEmpty(period_id)){
//            try {
//                connection = basicDataSource.getConnection();
//                String sql = "select 观测时间 from szpj_biz_水质指标观测数据 where 观测点_ID = ? order by 观测时间 desc";
//                ps = connection.prepareStatement(sql);
//                ps.setString(1, targetIds);
//                rs = ps.executeQuery();
//                if(rs.next()){
//                    startDate = sdf1.format((rs.getTimestamp("观测时间")));
////                    endDate = startDate;
//                }
//                if(startDate != null){
//                    String sql1 = "select t1.观测点_id,t3.rb_nm 断面名称,t1.观测时间,t1.水位,t1.流量,t1.水温,t1.气温 from szpj_biz_水文观测数据 t1,mgmb_河道断面信息简表 t2,wr_rb_b t3 where t1.观测时间 = to_date(?,'yyyy-mm-dd hh24:mi:ss') and t1.观测点_ID = t2.河道断面_ID and t3.rb_cd = t2.wr_sd_code and t1.观测点_id =?";
//                    ps1 = connection.prepareStatement(sql1);
//                    ps1.setString(1,startDate);
//                    ps1.setString(2,targetIds);
//                    rs1 = ps1.executeQuery();
//                    if(rs1.next()){
//                        ResultSetMetaData metaData1 = rs1.getMetaData();
//                        swmap = new LinkedHashMap<Object,Object>();
//                        for(int i = 0; i < metaData1.getColumnCount(); i++){
//                            String columnName = metaData1.getColumnName(i+1);
//                            if(rs1.getObject(columnName) != null){
//                                swmap.put(columnName,rs1.getObject(columnName));
//                            }
//                        }
////                        swList.add(swmap);
////                        list.add(swmap);
//                    }
//                    if(swmap!=null){
////                        for(int size = 0; size < swList.size(); size++){
//                            List<Map<Object,Object>> szList = new ArrayList<Map<Object,Object>>();
//                            String sql2 = "select t2.sys_name,t1.观测值,t1.取样位置 from szpj_biz_水质指标观测数据 t1,szpj_sys_指标项 t2 where 观测时间 = to_date(?,'yyyy-mm-dd hh24:mi:ss') and t1.观测item_id = t2.item_id and t1.观测点_id = ?";
//                            ps2 = connection.prepareStatement(sql2);
//                            ps2.setString(1,startDate);
//                            ps2.setString(2, (String) swmap.get("观测点_ID"));
//                            rs2 = ps2.executeQuery();
//                            ResultSetMetaData metaData2 = rs2.getMetaData();
//                            Set<Object> columnNames = new HashSet<Object>();
//                            while (rs2.next()){
//                                szMap = new LinkedHashMap<Object,Object>();
//                                for(int i = 0; i < metaData2.getColumnCount(); i++){
//                                    String columnName = metaData2.getColumnName(i+1);
//                                    szMap.put(columnName, rs2.getObject(columnName) != null ? rs2.getObject(columnName) : "");
//                                    if(rs2.getString(columnName) != null&&columnName.equals("SYS_NAME")){
//                                        columnNames.add(rs2.getObject(columnName));
//                                    }
//                                }
//                                szList.add(szMap);
//                            }
//                            //处理指标项相同的观测值
//                            Iterator name = columnNames.iterator();
//                            Map map3 = new LinkedHashMap();
//                            swmap.remove("观测点_ID");
//                            swmap.remove("断面名称");
//                            String time = sdf.format((java.util.Date)swmap.get("观测时间"));
//                            swmap.put("观测时间", time);
//                            map3.putAll(swmap);
//                            while (name.hasNext()){
//                                List list1 = new ArrayList();
//                                String currentName = (String) name.next();
//                                for(int i = 0; i < szList.size();i++){
//                                    Map map1 = szList.get(i);
//                                    //如果有指标相同的项就存放到list1中
//                                    if(map1.get("SYS_NAME").equals(currentName)){
//                                        list1.add(map1);
//                                    }
//                                }
//                                //如果list1的大于0，说明有相同的指标项
//                                if(list1.size()>0){
//                                    double avgGcValue = 0;
//                                    double gcValue = 0;
//                                    int count = list1.size();
//                                    for(int j = 0 ; j < list1.size();j++){
//                                        Map map2 = (Map) list1.get(j);
//                                        BigDecimal value = (BigDecimal) map2.get("观测值");
//                                        gcValue = gcValue + value.doubleValue();
//                                    }
//                                    avgGcValue = gcValue/count;
//                                    map3.put(currentName,avgGcValue);
//                                }//else{
////                                    Map map4 = (Map)list1.get(0);
////                                    map3.put(currentName,map4.get("观测值"));
////                                }
//                            }
//                            list.add(map3);
////                        }
//                    }
//                }
//            } catch (SQLException e) {
//                e.printStackTrace();
//                throw e;
//            }finally {
//                rs.close();
//                if(rs1!=null){
//                    rs1.close();
//                }
//                if(rs2!=null){
//                    rs2.close();
//                }
//                ps.close();
//                if(ps1!=null){
//                    ps1.close();
//                }
//                if(ps2!=null){
//                    ps2.close();
//                }
//                connection.close();
//            }
//        }else{
//            Pattern pattern = Pattern.compile("(\\d{4})(-)(\\d{2})(-)(\\d{2})(~)(\\d{4})(-)(\\d{2})(-)(\\d{2})");
//            Matcher matcher = pattern.matcher(period_id);
//            if(matcher.find()){
//                //符合yyyy-mm-dd~yyyy-mm-dd格式的日期
//                String[] date = period_id.split("~");
//                startDate = date[0];
//                endDate = date[1];
//            }else{
//                //传过来的是周期ID
//                connection = basicDataSource.getConnection();
//                call = connection.prepareCall("{call SP_GETBEGENDDATE(?,?,?)}");
//                call.setString(1, period_id);
//                call.setString(2, searchType);
//                call.registerOutParameter(3, OracleTypes.CURSOR);
//                call.execute();
//                rs = (ResultSet) call.getObject(3);
//                if (rs.next()){
//                    startDate = sdf.format(rs.getDate(1));
//                    endDate = sdf.format(rs.getDate(2));
//                }
//                rs.close();
//                call.close();
//                connection.close();
//            }
//            //得到日期后，再获取第三个结果集
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            call.setString(1,water_body_id);
//            call.setString(2,searchType);
//            call.setString(3,targetIds);
//            call.setString(4, startDate);
//            call.setString(5, endDate);
//            call.registerOutParameter(6, OracleTypes.CURSOR);
//            call.registerOutParameter(7, OracleTypes.CURSOR);
//            call.registerOutParameter(8, OracleTypes.CURSOR);
//            call.execute();
//            rs = (ResultSet) call.getObject(8);
//            Map<Object,Object> map = null;
//            while (rs.next()){
//                map = new HashMap<Object,Object>();
//                map.put("指标ID",rs.getString("观测item_id"));
//                map.put("指标名称",rs.getString("SYS_NAME"));
//                map.put("测量值",rs.getDouble("观测值"));
//                map.put("评价结果",rs.getString("指标类别"));
//                map.put("超标情况", rs.getDouble("超标倍数") > 0 ? rs.getDouble("超标倍数") : "");
//                list.add(map);
//            }
//            connection.commit();
//            if(rs != null)
//                rs.close();
//            if(call != null)
//                call.close();
//            if(connection != null)
//                connection.close();
//        }
//        return list;
//    }

    @Override
    public List<ShuiZiYuanBody> getObjsBySearch(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETOBJS(?,?,?,?,?,?)}");
            call.setString(1, userId);
            call.setString(2, waterBodyId);
            call.setString(3, mainTypeId);
            call.setString(4, typeIds);
            call.setString(5, keyword);
            call.registerOutParameter(6, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(6);
            List<ShuiZiYuanBody> list = new ArrayList<ShuiZiYuanBody>();
            while (rs.next()) {
                ShuiZiYuanBody shuiZiYuanBody = new ShuiZiYuanBody();
                shuiZiYuanBody.setId(rs.getString(1));
                shuiZiYuanBody.setName(rs.getString(2));
                shuiZiYuanBody.setLocation(rs.getString(3));
                shuiZiYuanBody.setMapId(rs.getString(4));
                shuiZiYuanBody.setSuperObjId(rs.getString(5));
                shuiZiYuanBody.setIcoFlag(rs.getString(6));
                String str = "";
                for (int i = 7; i <= 10; i++) {
                    if (rs.getString(i) != null && !"".equals(rs.getString(i))) {
                        str += rs.getString(i);
                        str += ",";
                    }
                }
                if (str.length() > 0)
                    shuiZiYuanBody.setTypeIDs(str.substring(0, str.length() - 1));
                list.add(shuiZiYuanBody);
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<ShuiZiYuanBody> getObjs(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETOBJS(?,?,?,?,?,?)}");
            call.setString(1, userId);
            call.setString(2, waterBodyId);
            call.setString(3, mainTypeId);
            call.setString(4, typeIds);
            call.setString(5, keyword);
            call.registerOutParameter(6, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(6);
            List<ShuiZiYuanBody> list = new ArrayList<ShuiZiYuanBody>();
            while (rs.next()) {
                ShuiZiYuanBody shuiZiYuanBody = new ShuiZiYuanBody();
                shuiZiYuanBody.setId(rs.getString(1));
                shuiZiYuanBody.setName(rs.getString(2));
                shuiZiYuanBody.setLocation(rs.getString(3));
                shuiZiYuanBody.setMapId(rs.getString(4));
                shuiZiYuanBody.setSuperObjId(rs.getString(5));
                shuiZiYuanBody.setIcoFlag(rs.getString(6));
                String str = "";
                for (int i = 7; i <= 10; i++) {
                    if (rs.getString(i) != null && !"".equals(rs.getString(i))) {
                        str += rs.getString(i);
                        str += ",";
                    }
                }
                if (str.length() > 0)
                    shuiZiYuanBody.setTypeIDs(str.substring(0, str.length() - 1));
                list.add(shuiZiYuanBody);
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<String, String>> getObjDetailInfo(String objId) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;

        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETOBJINFO(?,?)}");
            call.setString(1, objId);
            call.registerOutParameter(2, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(2);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            ResultSetMetaData metas = rs.getMetaData();
            Map<String, String> map = null;
            while (rs.next()) {
                for (int i = 0; i < metas.getColumnCount(); i++) {
                    map = new HashMap<String, String>();
                    String name = metas.getColumnName(i + 1);
                    map.put("name", name);
                    map.put("value", rs.getString(name) != null ? rs.getString(name) : "");
                    list.add(map);
                }
            }
            return list;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

//    @Override
//    public List<Map<Object, Object>> getPollutionItems2(String objId, String water_body_id, String searchType, String period_id) throws SQLException {
//        Connection connection = null;
//        CallableStatement call = null;
//
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//
//        String startDate = null;
//        String endDate = null;
//        //存放的是最后的结果
//        List sumList = new ArrayList();
//        //存放污染物的项
//        List<Map<Object,Object>> list = new ArrayList<Map<Object,Object>>();
//        //存放断面信息
//        Map m1 = null;
//        Map m2 = null;
//        Pattern pattern = Pattern.compile("(\\d{4})(-)(\\d{2})(-)(\\d{2})(~)(\\d{4})(-)(\\d{2})(-)(\\d{2})");
//        Matcher matcher = pattern.matcher(period_id);
//        if(matcher.find()){
//            //符合yyyy-mm-dd~yyyy-mm-dd格式的日期
//            String[] date = period_id.split("~");
//            startDate = date[0];
//            endDate = date[1];
//        }else{
//            //传过来的是周期ID
//            connection = basicDataSource.getConnection();
//            call = connection.prepareCall("{call SP_GETBEGENDDATE(?,?,?)}");
//            call.setString(1, period_id);
//            call.setString(2, searchType);
//            call.registerOutParameter(3, OracleTypes.CURSOR);
//            call.execute();
//            rs1 = (ResultSet) call.getObject(3);
//            if (rs1.next()){
//                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//                startDate = sdf.format(rs1.getDate(1));
//                endDate = sdf.format(rs1.getDate(2));
//            }
//            rs1.close();
//            call.close();
//            connection.close();
//        }
//        connection = basicDataSource.getConnection();
//        call = connection.prepareCall("{call SP_GETOBJINFO(?,?)}");
//        call.setString(1, objId);
//        call.registerOutParameter(2, OracleTypes.CURSOR);
//        call.execute();
//        rs1 = (ResultSet) call.getObject(2);
//        ResultSetMetaData metaData = rs1.getMetaData();
//        List<String> name = null;
//        if(rs1.next()){
//            m2 = new HashMap();
//            name = new ArrayList<String>();
//            for(int i = 0; i < metaData.getColumnCount(); i++){
//                String columnName = metaData.getColumnName(i+1);
//                name.add(columnName);
//            }
//            if(name.contains("断面代码")){
//                m2.put("dmCode",rs1.getString("断面代码"));
//            }
//        }
//        rs1.close();
//        call.close();
//        connection.close();
//        if(m2.size()>0){
//            try {
//                connection = basicDataSource.getConnection();
//                connection.setAutoCommit(false);
//                call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//                call.setString(1,water_body_id);
//                call.setString(2,searchType);
//                call.setString(3,objId);
//                call.setString(4, startDate);
//                call.setString(5, endDate);
//                call.registerOutParameter(6, OracleTypes.CURSOR);
//                call.registerOutParameter(7, OracleTypes.CURSOR);
//                call.registerOutParameter(8, OracleTypes.CURSOR);
//                call.execute();
//                rs1 = (ResultSet) call.getObject(6);
//                rs2 = (ResultSet) call.getObject(8);
//                //totalList存放查询出来的结果集
//                List<Map<Object,Object>> totalList = new ArrayList<Map<Object,Object>>();
//                Map<Object,Object> map = null;
//
//                if(rs1.next()){
//                    String levelId = (String) rs1.getObject("水质状况");
//                    if(!"优".equals(levelId)&&!"良好".equals(levelId)){
//                        m1 = new HashMap<Object,Object>();
//                        m1.putAll(m2);
//                        m1.put("name",rs1.getString("WR_SD_NM"));
//                        m1.put("waterGrade",rs1.getString("水质状况"));
//                        while (rs2.next()){
//                            //开始处理返回的主要污染指标结果集
//                            map = new HashMap<Object,Object>();
//                            map.put("itemName",rs2.getString("SYS_NAME"));
//                            map.put("metalFlag",rs2.getString("重金属标志"));
//                            map.put("gcValue",rs2.getString("观测值"));
//                            map.put("itemType",rs2.getString("指标类别"));
//                            map.put("pollutionTimes", rs2.getDouble("超标倍数"));
//                            totalList.add(map);
//                        }
//                        //所有重金属标志污染超标的都需要返回，如果重金属标志污染超标的小于三个，则再进行从非重金属里找出污染超标的
//                        //metalList存放所有重金属标志的项
//                        //notMetalList存放所有非重金属标志的项
//                        List<Map<Object,Object>> metalList = new ArrayList<Map<Object,Object>>();
//                        List<Map<Object,Object>> notMetalList = new ArrayList<Map<Object,Object>>();
//                        for(int i = 0; i < totalList.size() ; i++){
//                            if("1".equals(totalList.get(i).get("metalFlag"))){
//                                metalList.add(totalList.get(i));
//                            }else{
//                                notMetalList.add(totalList.get(i));
//                            }
//                        }
//                        //判断重金属标志污染超标的有几项，大于等于三项，排序之后就可以返回；如果小于三项就继续处理
//                        //1、overStandardMetalList存放重金属标志污染超标项
//                        List<Map<Object,Object>> overStandardMetalList1 = new ArrayList<Map<Object,Object>>();
//                        List<Map<Object,Object>> overStandardMetalList2 = new ArrayList<Map<Object,Object>>();
//                        List<Map<Object,Object>> overStandardMetalList3 = new ArrayList<Map<Object,Object>>();
//                        for(int j = 0; j < metalList.size() ; j++){
//                            if("IV类".equals(metalList.get(j).get("itemType"))){
//                                overStandardMetalList1.add(metalList.get(j));
//                            }
//                            if("V类".equals(metalList.get(j).get("itemType"))){
//                                overStandardMetalList2.add(metalList.get(j));
//                            }
//                            if("VI类".equals(metalList.get(j).get("itemType"))){
//                                overStandardMetalList3.add(metalList.get(j));
//                            }
//                        }
//                        //2、判断超标的重金属总共有几个
//                        int size = overStandardMetalList1.size()+overStandardMetalList2.size()+overStandardMetalList3.size();
//                        if(size>=3){
//                            //只要某个超标项的list的个数超过一个就按照超标倍数排序
//                            if(overStandardMetalList1.size()>1){
//                                MapSortUtil.desc(overStandardMetalList1,"pollutionTimes");
//                            }
//                            if(overStandardMetalList1.size()>1){
//                                MapSortUtil.desc(overStandardMetalList2,"pollutionTimes");
//                            }
//                            if(overStandardMetalList1.size()>1){
//                                MapSortUtil.desc(overStandardMetalList3,"pollutionTimes");
//                            }
//                            list.addAll(overStandardMetalList3);
//                            list.addAll(overStandardMetalList2);
//                            list.addAll(overStandardMetalList1);
//                        }else{
//                            //1、只要重金属超标项不为空，就把这些项放到tempList的最前边
//                            List<Map<Object,Object>> tempList = new ArrayList<Map<Object,Object>>();
//                            if(size!=0){
//                                if(overStandardMetalList3.size()>0){
//                                    tempList.addAll(overStandardMetalList3);
//                                }
//                                if(overStandardMetalList2.size()>0){
//                                    tempList.addAll(overStandardMetalList2);
//                                }
//                                if(overStandardMetalList1.size()>0){
//                                    tempList.addAll(overStandardMetalList1);
//                                }
//                            }
//
//                            //2、然后再找非重金属的超标项
//                            List<Map<Object,Object>> overStandardList1 = new ArrayList<Map<Object,Object>>();
//                            List<Map<Object,Object>> overStandardList2 = new ArrayList<Map<Object,Object>>();
//                            List<Map<Object,Object>> overStandardList3 = new ArrayList<Map<Object,Object>>();
//                            for(int z = 0; z < notMetalList.size() ; z++){
//                                if("IV类".equals(notMetalList.get(z).get("itemType"))){
//                                    overStandardList1.add(notMetalList.get(z));
//                                }
//                                if("V类".equals(notMetalList.get(z).get("itemType"))){
//                                    overStandardList2.add(notMetalList.get(z));
//                                }
//                                if("VI类".equals(notMetalList.get(z).get("itemType"))){
//                                    overStandardList3.add(notMetalList.get(z));
//                                }
//                            }
//                            //只要某个超标项的list的个数超过一个就按照超标倍数排序
//                            if(overStandardList1.size()>1){
//                                MapSortUtil.desc(overStandardList1,"pollutionTimes");
//                            }
//                            if(overStandardList2.size()>1){
//                                MapSortUtil.desc(overStandardList2,"pollutionTimes");
//                            }
//                            if(overStandardList3.size()>1){
//                                MapSortUtil.desc(overStandardList3,"pollutionTimes");
//                            }
//                            tempList.addAll(overStandardList3);
//                            tempList.addAll(overStandardList2);
//                            tempList.addAll(overStandardList1);
//                            if(tempList.size()>3){
//                                list.addAll(tempList.subList(0,3));
//                            }else{
//                                list.addAll(tempList);
//                            }
//
//                        }
//                        for(int m = 0; m < list.size();m++){
//                            list.get(m).remove("metalFlag");
//                        }
//                        sumList.add(m1);
//                        sumList.add(list);
//                    }
//                }
//
//
//            } catch (SQLException e) {
//                e.printStackTrace();
//                throw e;
//            }finally {
//                try {
//                    connection.commit();
//                    if(call != null)
//                        call.close();
//                    if(connection != null)
//                        connection.close();
//                } catch (SQLException e) {
//                    connection.rollback();
//                    e.printStackTrace();
//                    throw e;
//                }
//            }
//        }
//        return sumList;
//    }

    @Override
    public List<Map<String, String>> getSZPJDefaultObj(String waterBodyId) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETMONITOROBJ(?,?,?,?)}");
            call.setString(1, waterBodyId);
            call.setString(2, "断面");
            call.setString(3,"");
            call.registerOutParameter(4, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(4);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            Map<String, String> map = null;
            while (rs.next()) {
                map = new HashMap<String, String>();
                map.put("id", rs.getString("OBJ_ID"));
                map.put("name", rs.getString("名称"));
                map.put("location", rs.getString("CENTERXY"));
                map.put("mapId", rs.getString("MAP_ID"));
                map.put("superObjId", rs.getString("SUPER_OBJ_ID"));
                map.put("icoFlag", rs.getString("ICO_FLAG"));
                String str = "";
                map.put("typeIDs", rs.getString("SUPER_OBJ_ID"));
                if (rs.getString("MAIN_TYPEID") != null && !"".equals(rs.getString("MAIN_TYPEID"))) {
                    str += rs.getString("MAIN_TYPEID");
                }
                if (rs.getString("LEVEL_1_ID") != null && !"".equals(rs.getString("LEVEL_1_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_1_ID");
                }
                if (rs.getString("LEVEL_2_ID") != null && !"".equals(rs.getString("LEVEL_2_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_2_ID");
                }
                if (rs.getString("LEVEL_3_ID") != null && !"".equals(rs.getString("LEVEL_3_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_3_ID");
                }
                map.put("typeIDs",str);
                list.add(map);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<String, String>> getDefaultWaterFounc(String water_body_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "SELECT S.水功能区_ID,\n" +
                    "       V.SUPER_OBJ_ID,\n" +
                    "       S.水功能区名称,\n" +
                    "       S.水质目标名称,\n" +
                    "       D.COLOR_VALUE AS 目标水质颜色\n" +
                    "  FROM ZDWARNER.V_水功能区信息            S,\n" +
                    "       ZDWARNER.V_水资源对象              V,\n" +
                    "       ZDWARNER.szpj_sys_断面水质类别定义 D\n" +
                    " WHERE S.水功能区_ID = V.OBJ_ID\n" +
                    "   AND V.map_id = ?\n" +
                    "   AND S.水质目标ID = D.LEVEL_ID\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,"928FE3EF219048C5BFEF749785476164".equals(water_body_id)?"taizihe":"yuqiao");
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("waterFounctionId",rs.getString(1));
                resultMap.put("waterFounctionName",rs.getString(3));
                resultMap.put("superObjId",rs.getString(2));
                resultMap.put("goal",rs.getString(4));
                resultMap.put("color",rs.getString(5));
                resultList.add(resultMap);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return resultList;
    }

    @Override
    public List<Map<String, String>> getSZPJYearList(String water_body_id, String objIds) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETSZPJYEARLIST(?,?,?)}");
            call.setString(1, water_body_id);
            call.setString(2, objIds);
            call.registerOutParameter(3, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(3);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            Map<String, String> map = null;
            while (rs.next()) {
                map = new HashMap<String, String>();
                map.put("year", rs.getString(1));
                list.add(map);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<String, String>> getSZPJRiverList(String water_body_id, String p_in_year) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETSZPJRIVERLIST(?,?,?)}");
            call.setString(1, water_body_id);
            call.setString(2, p_in_year);
            call.registerOutParameter(3, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(3);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            Map<String, String> map = null;
            while (rs.next()) {
                map = new HashMap<String, String>();
                map.put("River_id", rs.getString(1));
                map.put("River_name", rs.getString(2));
                list.add(map);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<String, String>> getSZPJPeriodList(String water_body_id, String obj_ids, String year, String searchType) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            call = connection.prepareCall("{call SP_GETSZPJPERIODLIST(?,?,?,?,?)}");
            call.setString(1,water_body_id);
            call.setString(2,obj_ids);
            call.setString(3,year);
            call.setString(4, searchType);
            call.registerOutParameter(5, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(5);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            Map<String, String> map = null;
            while (rs.next()){
                map = new HashMap<String, String>();
                map.put("id",rs.getString(1));
                map.put("desc",rs.getString(2));
                list.add(map);
            }
            return list;

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                connection.commit();
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                connection.rollback();
                e.printStackTrace();
                throw e;
            }
        }
    }

//    @Override
//    public List<Map<String, Object>> getSZPJRiverLevel2(String water_body_id, String searchType, String period_id, String obj_ids) throws Exception {
//        Connection connection = null;
//        CallableStatement call = null;
//
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//
//        List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
//        List<Map<String,String>> list2 = new ArrayList<Map<String,String>>();
//
//        Map<String,Object> map1 = null;
//        Map<String,String> map2 = null;
//
//        String startDate = null;
//        String endDate = null;
//
//        Pattern pattern = Pattern.compile("(\\d{4})(-)(\\d{2})(-)(\\d{2})(~)(\\d{4})(-)(\\d{2})(-)(\\d{2})");
//        Matcher matcher = pattern.matcher(period_id);
//        if(matcher.find()){
//            //符合yyyy-mm-dd~yyyy-mm-dd格式的日期
//            String[] date = period_id.split("~");
//            startDate = date[0];
//            endDate = date[1];
//        }else{
//            //传过来的是周期ID
//            connection = basicDataSource.getConnection();
//            call = connection.prepareCall("{call SP_GETBEGENDDATE(?,?,?)}");
//            call.setString(1, period_id);
//            call.setString(2, searchType);
//            call.registerOutParameter(3, OracleTypes.CURSOR);
//            call.execute();
//            rs1 = (ResultSet) call.getObject(3);
//            if (rs1.next()){
//                SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//                startDate = sdf.format(rs1.getDate(1));
//                endDate = sdf.format(rs1.getDate(2));
//            }else{
//                throw new NullException("当前查询数据没有观测数据，无法查询，请重新查询");
//            }
//            rs1.close();
//            call.close();
//            connection.close();
//        }
//        try {
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            call.setString(1,water_body_id);
//            call.setString(2, searchType);
//            call.setString(3, obj_ids);
//            call.setString(4,startDate);
//            call.setString(5, endDate);
//            call.registerOutParameter(6, OracleTypes.CURSOR);
//            call.registerOutParameter(7, OracleTypes.CURSOR);
//            call.registerOutParameter(8, OracleTypes.CURSOR);
//            call.execute();
//            rs1 = (ResultSet) call.getObject(6);
//            rs2 = (ResultSet) call.getObject(7);
//            ResultSetMetaData metaData1 = rs1.getMetaData();
//            ResultSetMetaData metaData2 = rs2.getMetaData();
//            //详细信息（多条河流对应的详细信息）
//            while (rs2.next()){
//                map2 = new LinkedHashMap<String,String>();
//                for(int i = 0; i < metaData2.getColumnCount(); i++){
//                    String columnName = metaData2.getColumnName(i+1);
//                    map2.put(columnName, rs2.getString(columnName) != null ? rs2.getString(columnName) : "");
//                }
//                list2.add(map2);
//            }
//            //河流信息（多条）
//            while (rs1.next()){
//                map1 = new LinkedHashMap<String,Object>();
//                for(int i = 0; i < metaData1.getColumnCount();i++){
//                    String columnName = metaData1.getColumnName(i+1);
//                    map1.put(columnName, rs1.getString(columnName));
//                }
//                //根据河流的评价对象_id找到相对应的详细信息   list3
//                List<Map<String,String>> list3 = new ArrayList<Map<String,String>>();
//                for(int i = 0; i < list2.size(); i++){
//                    if(rs1.getString("评价对象_ID").equals(list2.get(i).get("RIVER_ID"))){
//                        list3.add(list2.get(i));
//                    }
//                }
//                map1.put("waterFunList", list3);
//                list.add(map1);
//            }
//            return list;
//        }catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                connection.commit();
//                rs1.close();
//                rs2.close();
//                if(rs1 != null)
//                    rs1.close();
//                if(rs2 != null)
//                    rs2.close();
//                if(call != null)
//                    call.close();
//                if(connection != null)
//                    connection.close();
//            } catch (SQLException e) {
//                connection.rollback();
//                e.printStackTrace();
//                throw e;
//            }
//        }
//    }

//    @Override
//    public List<Map<Object,Object>> getSZPJItemValue(String water_body_id, String searchType, String period_id, String obj_id) throws SQLException{
//        Connection connection = null;
//        CallableStatement call = null;
//
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//
//        String startDate = null;
//        String endDate = null;
//        //最后返回结果
//        List<Map<Object,Object>> list = new ArrayList<Map<Object,Object>>();
//
//        Map<Object,Object> map1 = new HashMap<Object,Object>();
//        List<Map<Object,Object>> dmList = new ArrayList<Map<Object,Object>>();
//
//        try {
//            connection = basicDataSource.getConnection();
//            Map<String,String> dateMap = getStartAndEndDate(period_id,searchType,connection);
//            startDate = dateMap.get("startDate");
//            endDate = dateMap.get("endDate");
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            call.setString(1,water_body_id);
//            call.setString(2,searchType);
//            call.setString(3,obj_id);
//            call.setString(4, startDate);
//            call.setString(5, endDate);
//            call.registerOutParameter(6, OracleTypes.CURSOR);
//            call.registerOutParameter(7, OracleTypes.CURSOR);
//            call.registerOutParameter(8, OracleTypes.CURSOR);
//            call.execute();
//            rs1 = (ResultSet) call.getObject(6);
//            rs2 = (ResultSet) call.getObject(8);
//            if(rs1.next()){
//                String levelID = (String) rs1.getObject("水质状况");
//                if(!"优".equals(levelID) && !"良好".equals(levelID)){
//                    //totalList存放查询出来的少于5个断面的结果集
//                    List<Map<Object,Object>> totalList = new ArrayList<Map<Object,Object>>();
//                    //sumList存放查询出来的多于5个断面的结果集
//                    List<Map<Object,Object>> sumList = new ArrayList<Map<Object,Object>>();
//
//                    Map<Object,Object> map = null;
//                    ResultSetMetaData metaData = rs2.getMetaData();
//                    while (rs2.next()){
//                        List<String> columnName = new ArrayList<String>();
//                        //根据返回的结果集是  超标倍数 还是  断面超标率 来决定用哪种计算方法
//                        for(int columnIndex = 0; columnIndex < metaData.getColumnCount() ; columnIndex++){
//                            columnName.add(metaData.getColumnName(columnIndex+1));
//                        }
//                        if(columnName.contains("超标倍数")){
//                            //开始处理返回的主要污染指标结果集
//                            map = new HashMap<Object,Object>();
//                            map.put("riverID",rs2.getString("RIVER_ID"));
//                            map.put("gcID",rs2.getString("观测点_ID"));
//                            map.put("断面名称",rs2.getString("断面名称"));
//                            map.put("污染物指标id",rs2.getString("观测ITEM_ID"));
//                            map.put("污染物名称",rs2.getString("SYS_NAME"));
//                            map.put("metalFlag",rs2.getString("重金属标志"));
//                            map.put("观测值",rs2.getString("观测值"));
//                            map.put("itemType",rs2.getString("指标类别"));
//                            map.put("超标倍数",rs2.getDouble("超标倍数"));
//                            totalList.add(map);
//                        }else if(columnName.contains("断面超标率")){
//                            map = new HashMap<Object,Object>();
//                            map.put("riverID",rs2.getString("RIVER_ID"));
//                            map.put("污染物指标id",rs2.getString("观测ITEM_ID"));
//                            map.put("污染物名称", rs2.getString("SYS_NAME"));
//                            map.put("metalFlag",rs2.getString("重金属标志"));
//                            map.put("断面超标率",rs2.getDouble("断面超标率"));
//                            sumList.add(map);
//                        }
//                    }
//                    //处理少于5个断面的数据
//                    if(totalList.size()>0){
//                        //含有超标倍数的集合中，1、先在totalList中找到有几条河流
//                        Set<Object> riverIDs = new HashSet<Object>();
//                        for(int size = 0; size < totalList.size();size++){
//                            riverIDs.add(totalList.get(size).get("riverID"));
//                        }
//                        //2、再找到河流中的断面
//                        Iterator riverID = riverIDs.iterator();
//                        while(riverID.hasNext()){
//                            List<Map<Object,Object>> riverDmList = new ArrayList<Map<Object,Object>>();
//                            String currentRiverID = (String) riverID.next();
//                            for(int size = 0; size < totalList.size();size++){
//                                if(totalList.get(size).get("riverID") .equals(currentRiverID)){
//                                    riverDmList.add(totalList.get(size));
//                                }
//                            }
//                            //将属于同一条河流的断面放到riverDmList后，开始处理属于同一条河流的同一个断面的所有指标项
//                            Set<Object> dmIDs = new HashSet<Object>();
//                            for(int size = 0; size < riverDmList.size();size++){
//                                dmIDs.add(riverDmList.get(size).get("gcID"));
//                            }
//                            Iterator dmID = dmIDs.iterator();
//                            while(dmID.hasNext()){
//                                List<Map<Object,Object>> riverDmItemsList = new ArrayList<Map<Object,Object>>();
//                                //存放的是一条河流下边的一个断面的详细信息
//                                Map<Object,Object> dmMap = new HashMap<Object,Object>();
//                                String currentDmID = (String) dmID.next();
//                                String currentDmName = null;
//                                for(int i = 0 ; i < riverDmList.size(); i++){
//                                    if(riverDmList.get(i).get("gcID").equals(currentDmID)){
//                                        riverDmItemsList.add(riverDmList.get(i));
//                                        if(currentDmName==null){
//                                            currentDmName = (String)riverDmList.get(i).get("断面名称");
//                                        }
//                                    }
//                                }
//
//                                //存放的是污染指标项或者反映的超标率的项
//                                List<Map<Object,Object>> list2 = new ArrayList<Map<Object,Object>>();
//                                //开始处理污染指标集合riverDmItemsList
//                                //所有重金属标志污染超标的都需要返回，如果重金属标志污染超标的小于三个，则再进行从非重金属里找出污染超标的
//                                //metalList存放所有重金属标志的项
//                                //notMetalList存放所有非重金属标志的项
//                                List<Map<Object,Object>> metalList = new ArrayList<Map<Object,Object>>();
//                                List<Map<Object,Object>> notMetalList = new ArrayList<Map<Object,Object>>();
//                                for(int i = 0; i < riverDmItemsList.size() ; i++){
//                                    if("1".equals(riverDmItemsList.get(i).get("metalFlag"))){
//                                        metalList.add(riverDmItemsList.get(i));
//                                    }else{
//                                        notMetalList.add(riverDmItemsList.get(i));
//                                    }
//                                }
//                                //判断重金属标志污染超标的有几项，大于等于三项，排序之后就可以返回；如果小于三项就继续处理
//                                //1、overStandardMetalList存放重金属标志污染超标项
//                                List<Map<Object,Object>> overStandardMetalList1 = new ArrayList<Map<Object,Object>>();
//                                List<Map<Object,Object>> overStandardMetalList2 = new ArrayList<Map<Object,Object>>();
//                                List<Map<Object,Object>> overStandardMetalList3 = new ArrayList<Map<Object,Object>>();
//                                for(int j = 0; j < metalList.size() ; j++){
//                                    if("IV类".equals(metalList.get(j).get("itemType"))){
//                                        overStandardMetalList1.add(metalList.get(j));
//                                    }
//                                    if("V类".equals(metalList.get(j).get("itemType"))){
//                                        overStandardMetalList2.add(metalList.get(j));
//                                    }
//                                    if("VI类".equals(metalList.get(j).get("itemType"))){
//                                        overStandardMetalList3.add(metalList.get(j));
//                                    }
//                                }
//                                //2、判断超标的重金属总共有几个
//                                int size = overStandardMetalList1.size()+overStandardMetalList2.size()+overStandardMetalList3.size();
//                                if(size>=3){
//                                    //只要某个超标项的list的个数超过一个就按照超标倍数排序
//                                    if(overStandardMetalList1.size()>1){
//                                        MapSortUtil.desc(overStandardMetalList1,"超标倍数","double");
//                                    }
//                                    if(overStandardMetalList1.size()>1){
//                                        MapSortUtil.desc(overStandardMetalList2,"超标倍数","double");
//                                    }
//                                    if(overStandardMetalList1.size()>1){
//                                        MapSortUtil.desc(overStandardMetalList3,"超标倍数","double");
//                                    }
//                                    list2.addAll(overStandardMetalList3);
//                                    list2.addAll(overStandardMetalList2);
//                                    list2.addAll(overStandardMetalList1);
//                                }else{
//                                    //1、只要重金属超标项不为空，就把这些项放到tempList的最前边
//                                    List<Map<Object,Object>> tempList = new ArrayList<Map<Object,Object>>();
//                                    if(size!=0){
//                                        if(overStandardMetalList3.size()>0){
//                                            tempList.addAll(overStandardMetalList3);
//                                        }
//                                        if(overStandardMetalList2.size()>0){
//                                            tempList.addAll(overStandardMetalList2);
//                                        }
//                                        if(overStandardMetalList1.size()>0){
//                                            tempList.addAll(overStandardMetalList1);
//                                        }
//                                    }
//
//                                    //2、然后再找非重金属的超标项
//                                    List<Map<Object,Object>> overStandardList1 = new ArrayList<Map<Object,Object>>();
//                                    List<Map<Object,Object>> overStandardList2 = new ArrayList<Map<Object,Object>>();
//                                    List<Map<Object,Object>> overStandardList3 = new ArrayList<Map<Object,Object>>();
//                                    for(int z = 0; z < notMetalList.size() ; z++){
//                                        if("IV类".equals(notMetalList.get(z).get("itemType"))){
//                                            overStandardList1.add(notMetalList.get(z));
//                                        }
//                                        if("V类".equals(notMetalList.get(z).get("itemType"))){
//                                            overStandardList2.add(notMetalList.get(z));
//                                        }
//                                        if("VI类".equals(notMetalList.get(z).get("itemType"))){
//                                            overStandardList3.add(notMetalList.get(z));
//                                        }
//                                    }
//                                    //只要某个超标项的list的个数超过一个就按照超标倍数排序
//                                    if(overStandardList1.size()>1){
//                                        MapSortUtil.desc(overStandardList1,"超标倍数","double");
//                                    }
//                                    if(overStandardList2.size()>1){
//                                        MapSortUtil.desc(overStandardList2,"超标倍数","double");
//                                    }
//                                    if(overStandardList3.size()>1){
//                                        MapSortUtil.desc(overStandardList3,"超标倍数","double");
//                                    }
//                                    tempList.addAll(overStandardList3);
//                                    tempList.addAll(overStandardList2);
//                                    tempList.addAll(overStandardList1);
//                                    if(tempList.size()>0){
//                                        if(tempList.size()>3){
//                                            list2.addAll(tempList.subList(0,3));
//                                        }else{
//                                            list2.addAll(tempList);
//                                        }
//                                    }
//                                }
//                                if(list2.size()>0){
//                                    dmMap.put("dmID",currentDmID);
//                                    dmMap.put("dmName",currentDmName);
//                                    dmMap.put("dmMessage", list2);
//                                    dmList.add(dmMap);
//                                }
//                            }
//                            map1.put("riverID",currentRiverID);
//                            map1.put("riverMessage",dmList);
//                            list.add(map1);
//                        }
//                        //处理得到的list数据
//                        for(int i = 0 ; i < list.size() ; i++){
//                            List list1  = (List) list.get(i).get("riverMessage");
//                            for(int m = 0 ; m < list1.size() ; m++){
//                                Map m1 = (Map)list1.get(m);
//                                List list2 = (List) m1.get("dmMessage");
//                                for(int j = 0 ; j < list2.size() ; j++){
//                                    Map m2 = (Map) list2.get(j);
//                                    m2.remove("gcID");
//                                    m2.remove("riverID");
//                                    m2.remove("itemType");
//                                    m2.remove("metalFlag");
//                                    m2.remove("断面名称");
//                                }
//                            }
//                        }
//
//                    }
//                    //处理多于5个断面的数据
//                    if(sumList.size()>0){
//                        //含有断面超标率的集合中，1、先在sumList中找到有几条河流
//                        Set<Object> riverIDs = new HashSet<Object>();
//                        for(int size = 0; size < sumList.size();size++){
//                            riverIDs.add(sumList.get(size).get("riverID"));
//                        }
//                        //2、再根据河流ID找到所有指标项
//                        Iterator riverID = riverIDs.iterator();
//                        while(riverID.hasNext()){
//                            String currentRiverID = (String)riverID.next();
//                            //存放的是一条河流下边的一个断面的详细信息
//                            Map<Object,Object> dmMap = new HashMap<Object,Object>();
//                            //itemList存放找到的所有指标项
//                            List<Map<Object,Object>> itemList = new ArrayList<Map<Object, Object>>();
//                            for(int i = 0; i < sumList.size();i++){
//                                if(sumList.get(i).get("riverID").equals(currentRiverID)){
//                                    itemList.add(sumList.get(i));
//                                }
//                            }
//                            //开始处理当前河流的相关的指标项，如果是重金属标志的，所有的超标率数据都返回
//                            List<Map<Object,Object>> dmMetalOverStandardList = new ArrayList<Map<Object,Object>>();
//                            List<Map<Object,Object>> dmNotMetalOverStandardList = new ArrayList<Map<Object,Object>>();
//                            for(int i = 0 ; i < itemList.size();i++){
//                                if("1".equals(itemList.get(i).get("metalFlag"))){
//                                    dmMetalOverStandardList.add(itemList.get(i));
//                                }else {
//                                    dmNotMetalOverStandardList.add(itemList.get(i));
//                                }
//                            }
//                            //如果dmMetalOverStandardList超过5条记录，全部返回重金属的数据
//                            if(dmMetalOverStandardList.size()>=3){
//                                MapSortUtil.desc(dmMetalOverStandardList,"断面超标率","double");
//                                dmMap.put("dmID", "$$$");
//                                dmMap.put("dmName","$$$");
//                                dmMap.put("dmMessage", dmMetalOverStandardList);
//                                map1.put("riverID",currentRiverID);
//                                map1.put("riverMessage",dmMap);
//                                list.add(map1);
//                            }else{
//                                //1、只要重金属标志的记录不为空，就把这些项放到tempList的最前边
//                                List<Map<Object,Object>> tempList = new ArrayList<Map<Object,Object>>();
//                                if(dmMetalOverStandardList.size()>0){
//                                    MapSortUtil.desc(dmMetalOverStandardList,"断面超标率","double");
//                                    tempList.addAll(dmMetalOverStandardList);
//                                }
//                                //2、非重金属标志的记录不为空
//                                MapSortUtil.desc(dmNotMetalOverStandardList,"断面超标率","double");
//                                tempList.addAll(dmNotMetalOverStandardList);
//                                if(tempList.size()>0){
//                                    dmMap.put("dmID","$$$");
//                                    dmMap.put("dmName","$$$");
//                                    dmMap.put("dmMessage",tempList.size()>2?tempList.subList(0,3):tempList);
//                                    map1.put("riverID",currentRiverID);
//                                    map1.put("riverMessage",dmMap);
//                                    list.add(map1);
//                                }
//                            }
//                        }
//                        //处理得到的多于5个断面的list
//                        DecimalFormat df  = new DecimalFormat("######0.00");
//                        for(int i = 0;i < list.size();i++){
//                            Map m1 = (Map) list.get(i).get("riverMessage");
//                            List list1 = (List) m1.get("dmMessage");
//                            for(int j = 0 ; j < list1.size();j++){
//                                Map m2 = (Map)list1.get(j);
//                                m2.remove("riverID");
//                                m2.remove("metalFlag");
//                                m2.put("断面超标率", df.format((Double) m2.get("断面超标率") * 100) + "%");
//                            }
//
//                        }
//                    }
//                }
//            }
//
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                connection.commit();
//                if(rs1 != null)
//                    rs1.close();
//                if(rs2 != null)
//                    rs2.close();
//                if(call != null)
//                    call.close();
//                if(connection != null)
//                    connection.close();
//            } catch (SQLException e) {
//                connection.rollback();
//                e.printStackTrace();
//                throw e;
//            }
//        }
//        return list;
//
//    }

    @Override
    public Map<String, Object> getSZPJGgraphs(String period_id, String dmId, String itemId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String startDate = null;
        String endDate = null;
        SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        Pattern pattern = Pattern.compile("(\\d{4})(-)(\\d{2})(-)(\\d{2})(~)(\\d{4})(-)(\\d{2})(-)(\\d{2})");
        Matcher matcher = pattern.matcher(period_id);
        try {
            connection = basicDataSource.getConnection();
            if(matcher.find()){
                //符合yyyymmdd-yyyymmdd格式的日期
                String[] date = period_id.split("~");
                startDate = date[0];
                endDate = date[1];
            }else{
                //传过来的是周期ID
                String sql = "select beg_date,end_date from szpj_sys_周期定义表 where sn_id = ?";
                ps = connection.prepareStatement(sql);
                ps.setString(1,period_id);
                rs = ps.executeQuery();
                if (rs.next()){
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                    startDate = sdf.format(rs.getDate(1));
                    endDate = sdf.format(rs.getDate(2));
                }
            }
            String sql = "select t.观测点_id, t.观测item_id, t.观测时间, t.观测值\n" +
                    "  from szpj_biz_水质指标观测数据 t\n" +
                    " where t.观测点_id = ?\n" +
                    "   and t.观测item_id = ?\n" +
                    "   and t.观测时间 >= to_date(?, 'yyyy-MM-dd')\n" +
                    "   and t.观测时间 <= to_date(?, 'yyyy-MM-dd')\n" +
                    " order by t.观测时间\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,dmId);
            ps.setString(2,itemId);
            ps.setString(3,startDate);
            ps.setString(4,endDate);
            rs = ps.executeQuery();
            List<Map<String,String>> list = new ArrayList<Map<String, String>>();
            while (rs.next()){
                Map<String,String> map = new LinkedHashMap<String,String>();
                map.put("observeTime",sdf1.format(new Date(rs.getTimestamp(3).getTime())));
                map.put("observeValue",rs.getString(4));
                list.add(map);
            }
            resultMap.put("startDate", startDate);
            resultMap.put("endDate", endDate);
            resultMap.put("dmId", dmId);
            resultMap.put("itemId",itemId);
            resultMap.put("observeInfo",list);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return resultMap;
    }

    //存放水功能区，控制断面，行政区所有信息
    public List<Map<Object,Object>> getxzqList(String water_body_id , String xzq_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放水功能区，控制断面，行政区所有信息
        List<Map<Object,Object>> xzqList = new ArrayList<Map<Object,Object>>();
        //存放水功能区，控制断面，行政区单条信息
        Map<Object,Object> xzqMap = null;
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t1.水功能区_id,\n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n" +
                    "       t4.WFZ_NM\n" +
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id = ?\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n" +
                    "union\n" +
                    "select t1.水功能区_id,\n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n" +
                    "       t4.WFZ_NM\n" +
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, xzq_id);
            ps.setString(2, water_body_id);
            ps.setString(3, xzq_id);
            ps.setString(4,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                xzqMap = new HashMap<Object,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    xzqMap.put(columnName,rs.getObject(columnName));
                }
                xzqList.add(xzqMap);
            }
        } catch (SQLException e) {
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

        return xzqList;
    }

//    @Override
//    public Map<String, Object> getSzyWaterQuality2(List<Map<Object,Object>> xzqList,String water_body_id , String xzq_id, String year) throws SQLException {
//        Connection connection = null;
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//        String startDate = year + "-01-01";
//        String endDate = year + "-12-31";
//        //存放所属行政区内的达标水功能区的个数达标率和长度达标率
////        List<Map<String,String>> list = new ArrayList<Map<String,String>>();
//        Map<String,Object> reachMap = null;
//
//        //存放水功能区，控制断面，行政区所有信息
////        List<Map<Object,Object>> xzqList = getxzqList(water_body_id,xzq_id);
//        //存放水功能区，控制断面，行政区单条信息
////        Map<Object,Object> xzqMap = null;
//
//        //存放所有控制断面的年度观测情况信息
//        List totalDmList = new ArrayList();
//        //存放每个控制断面年度观测情况信息
//        List<Map<Object,Object>> dmList = null;
//        //存放每个控制断面年度观测情况一条信息
//        Map<Object,Object> dmMap = null;
//
//        //存放所有重组控制断面的年度观测情况信息(化学需氧量)
//        List newTotalDmCODList = new ArrayList();
//        //存放每个重组控制断面的年度观测情况信息
//        List newDmCODList = null;
//        //存放每个重组控制断面年度观测情况一条信息
////        Map newDmCODMap = null;
//
//        //存放所有重组控制断面的年度观测情况信息(氨氮)
//        List newTotalDmNHList = new ArrayList();
//        //存放每个重组控制断面的年度观测情况信息
//        List newDmNHList = null;
//        //存放每个重组控制断面年度观测情况一条信息
////        Map newDmNHMap = null;
//        //开始进行水资源承载力之水功能区水质达标率
//        try {
//            //先通过行政区_ID找出对应的水功能区信息
//            connection = basicDataSource.getConnection();
//            //通过xzqList里边的河道断面_ID也就是水功能区里边的控制断面找到当前年份的氨氮(NH3-N)和化学需氧量(COD)的相关信息
//            String sql2 = "select t1.观测时间,t1.观测值,t2.sys_name from szpj_biz_水质指标观测数据 t1,szpj_sys_指标项 t2 where t1.观测item_id = t2.item_id and t2.sys_name in ('氨氮(NH3-N)','化学需氧量(COD)') and to_char(t1.观测时间,'yyyy-mm-dd') between ? and ? and t1.观测点_id = ?";
//            ps = connection.prepareStatement(sql2);
//            ps.setString(1,startDate);
//            ps.setString(2,endDate);
//            for(int size = 0 ; size < xzqList.size() ; size++){
//                dmList = new ArrayList<Map<Object,Object>>();
//                ps.setString(3, (String) xzqList.get(size).get("河道断面_ID"));
//                rs = ps.executeQuery();
//                ResultSetMetaData metaData2 = rs.getMetaData();
//                while (rs.next()){
//                    dmMap = new HashMap<Object,Object>();
//                    for(int i = 0 ; i < metaData2.getColumnCount() ; i++){
//                        String columnName = metaData2.getColumnName(i+1);
//                        dmMap.put(columnName,rs.getObject(columnName));
//                    }
//                    dmMap.put("水功能区ID",xzqList.get(size).get("水功能区_ID"));
//                    dmMap.put("水质目标",xzqList.get(size).get("水质目标"));
//                    dmMap.put("行政区划名称", xzqList.get(size).get("行政区划名称"));
//                    dmMap.put("水功能区经纬度",xzqList.get(size).get("经纬度坐标"));
//                    dmMap.put("水功能区名称",xzqList.get(size).get("WFZ_NM"));
////                    dmMap.put("上游长度",xzqList.get(size).get("上游长度"));
//                    dmList.add(dmMap);
//                }
//                if(dmList.size()>0){
//                    totalDmList.add(dmList);
//                }
//            }
//            //将totalDmList中的观测时间的时间戳格式改为年月日格式
//            if(totalDmList.size()>0) {
//                for (int i = 0; i < totalDmList.size(); i++) {
//                    List list1 = (List) totalDmList.get(i);
//                    for (int j = 0; j < list1.size(); j++) {
//                        Map map = (Map) list1.get(j);
//                        String date = sdf.format((java.util.Date) map.get("观测时间"));
//                        map.put("观测时间", date);
//                    }
//                }
//                //计算所有控制断面中每个控制断面的当前年份的监测次数
//                for (int i = 0; i < totalDmList.size(); i++) {
//                    Set dateSet = new HashSet();
//                    List list1 = (List) totalDmList.get(i);
//                    for (int j = 0; j < list1.size(); j++) {
//                        Map map = (Map) list1.get(j);
//                        dateSet.add(map.get("观测时间"));
//                    }
//                    //将观测时间相同的观测值做平均值
//                    if (dateSet.size() > 0) {
//                        Iterator dates = dateSet.iterator();
//                        newDmCODList = new ArrayList();
//                        newDmNHList = new ArrayList();
//                        while (dates.hasNext()) {
//                            //存放观测时间相同的断面监测指标项的信息
//                            List sameTimeList1 = new ArrayList();
//                            List sameTimeList2 = new ArrayList();
//                            String currentDate = (String) dates.next();
//                            for (int k = 0; k < list1.size(); k++) {
//                                Map map = (Map) list1.get(k);
//                                if (map.get("观测时间").equals(currentDate)) {
//                                    if (map.get("SYS_NAME").equals("化学需氧量(COD)")) {
//                                        sameTimeList1.add(map);
//                                    }
//                                    if (map.get("SYS_NAME").equals("氨氮(NH3-N)")) {
//                                        sameTimeList2.add(map);
//                                    }
//                                }
//                            }
//                            if (sameTimeList1.size() > 0) {
//                                double gcSumValue = 0;
//                                double gcAvgValue = 0;
//                                String waterID = null;
//                                String xzqName = null;
//                                String goal = null;
//                                String gcTime = null;
//                                String itemName = null;
//                                for (int m = 0; m < sameTimeList1.size(); m++) {
//                                    Map map = (Map) sameTimeList1.get(m);
//                                    if (StringUtils.isEmpty(waterID)) {
//                                        waterID = (String) map.get("水功能区ID");
//                                    }
//                                    if (StringUtils.isEmpty(xzqName)) {
//                                        xzqName = (String) map.get("行政区划名称");
//                                    }
//                                    if (StringUtils.isEmpty(goal)) {
//                                        goal = (String) map.get("水质目标");
//                                    }
//                                    if (StringUtils.isEmpty(gcTime)) {
//                                        gcTime = (String) map.get("观测时间");
//                                    }
//                                    if (StringUtils.isEmpty(itemName)) {
//                                        itemName = (String) map.get("SYS_NAME");
//                                    }
//                                    BigDecimal gcValue = (BigDecimal) map.get("观测值");
//                                    gcSumValue += gcValue.doubleValue();
//                                }
//                                gcAvgValue = gcSumValue / sameTimeList1.size();
//                                Map newDmCODMap = new LinkedHashMap();
//                                newDmCODMap.put("水功能区ID", waterID);
//                                newDmCODMap.put("行政区划名称", xzqName);
//                                newDmCODMap.put("水质目标", goal);
//                                newDmCODMap.put("观测时间", gcTime);
//                                newDmCODMap.put("指标项", itemName);
//                                newDmCODMap.put("观测值", gcAvgValue);
//                                newDmCODList.add(newDmCODMap);
//                            }
//                            if (sameTimeList2.size() > 0) {
//                                double gcSumValue = 0;
//                                double gcAvgValue = 0;
//                                String waterID = null;
//                                String xzqName = null;
//                                String goal = null;
//                                String gcTime = null;
//                                String itemName = null;
//                                for (int m = 0; m < sameTimeList2.size(); m++) {
//                                    Map map = (Map) sameTimeList2.get(m);
//                                    if (StringUtils.isEmpty(waterID)) {
//                                        waterID = (String) map.get("水功能区ID");
//                                    }
//                                    if (StringUtils.isEmpty(xzqName)) {
//                                        xzqName = (String) map.get("行政区划名称");
//                                    }
//                                    if (StringUtils.isEmpty(goal)) {
//                                        goal = (String) map.get("水质目标");
//                                    }
//                                    if (StringUtils.isEmpty(gcTime)) {
//                                        gcTime = (String) map.get("观测时间");
//                                    }
//                                    if (StringUtils.isEmpty(itemName)) {
//                                        itemName = (String) map.get("SYS_NAME");
//                                    }
//                                    BigDecimal gcValue = (BigDecimal) map.get("观测值");
//                                    gcSumValue += gcValue.doubleValue();
//                                }
//                                gcAvgValue = gcSumValue / sameTimeList2.size();
//                                Map newDmNHMap = new LinkedHashMap();
//                                newDmNHMap.put("水功能区ID", waterID);
//                                newDmNHMap.put("行政区划名称", xzqName);
//                                newDmNHMap.put("水质目标", goal);
//                                newDmNHMap.put("观测时间", gcTime);
//                                newDmNHMap.put("指标项", itemName);
//                                newDmNHMap.put("观测值", gcAvgValue);
//                                newDmNHList.add(newDmNHMap);
//                            }
//                        }
//                        if(newDmCODList!=null&&newDmCODList.size()>0){
//                            newTotalDmCODList.add(newDmCODList);
//                        }
//                        if(newDmNHList!=null&&newDmNHList.size()>0){
//                            newTotalDmNHList.add(newDmNHList);
//                        }
//                    }
//                }
//                //开始计算比较是否达标
//                //因为该行政区内的水功能区的控制断面的监测指标项不一定有两项，所以应该根据totalDmList里边包含的水功能区来分别查找
//                //newTotalDmCODList和newTotalDmNHList的大小，取最大的那个作为该水功能区的监测次数
//                //存放达标的水功能区ID
//                List<String> waterFunctionIDs = new ArrayList<String>();
//                String name = null;
//                //存放某一条河流某一个行政区内的所有水功能区信息
//                List waterFounctionList = new ArrayList();
//                for(int i = 0 ; i < totalDmList.size() ; i++){
//                    List list1 = (List) totalDmList.get(i);
//                    //找到当前的水功能区ID
//                    String waterFounctionID = null;
//                    String waterFounctionName = null;
//                    //存放某一条河流某一个行政区内的某一个水功能区信息
//                    Map waterFounctionMap = new LinkedHashMap();
//                    for(int j = 0 ; j < list1.size() ; j++){
//                        Map map1 = (Map) list1.get(j);
//                        name = (String)map1.get("行政区划名称");
//                        waterFounctionID = (String) map1.get("水功能区ID");
//                        waterFounctionName = (String)map1.get("水功能区名称");
//                    }
//                    waterFounctionMap.put("funCode",waterFounctionID);
//                    waterFounctionMap.put("funName",waterFounctionName);
//                    //根据当前的水功能区ID查找newTotalDmCODList和newTotalDmNHList，确定监测次数
//                    //存放当前水功能区ID相对应的COD指标项的list
//                    List listCOD = new ArrayList();
//                    //存放当前水功能区ID相对应的NH指标项的list
//                    List listNH = new ArrayList();
//                    for(int j = 0 ; j < newTotalDmCODList.size(); j++){
//                        List list2 = (List) newTotalDmCODList.get(j);
//                        for(int k = 0 ; k < list2.size() ; k++){
//                            Map map2 = (Map) list2.get(k);
//                            if(waterFounctionID.equals((String)map2.get("水功能区ID"))){
//                                if(listCOD.size()==0){
//                                    listCOD = list2;
//                                }
//                            }else{
//                                break;
//                            }
//                        }
//                    }
//                    for(int j = 0 ; j < newTotalDmNHList.size(); j++){
//                        List list2 = (List) newTotalDmNHList.get(j);
//                        for(int k = 0 ; k < list2.size() ; k++){
//                            Map map2 = (Map) list2.get(k);
//                            if(waterFounctionID.equals((String)map2.get("水功能区ID"))){
//                                if(listNH.size()==0){
//                                    listNH = list2;
//                                }
//                            }else{
//                                break;
//                            }
//                        }
//                    }
//                    //比较listCOD和listNH的大小，取最大值count作为监测次数
//                    int count = 0;
//                    double countDB = 0;
//                    if(listCOD.size()>=listNH.size()){
//                        count = listCOD.size();
//                    }else{
//                        count = listNH.size();
//                    }
//                    //根据监测次数的不同来进行不同的达标的计算
//                    if(count>=6){
//                        //监测次数大于等于6次，平均观测值和III类水质比较
//                        double gcCODValue = 0;
//                        double avgGcCODValue = 0;
//                        double gcNHValue = 0;
//                        double avgGcNHValue = 0;
//                        boolean flag1 = false;
//                        boolean flag2 = false;
//                        Set<String> dates = new HashSet<String>();
//                        for(int m = 0 ; m < listCOD.size() ; m++){
//                            Map map3 = (Map) listCOD.get(m);
//                            dates.add((String) map3.get("观测时间"));
//                            gcCODValue = gcCODValue + (double)map3.get("观测值");
//                        }
//                        for(int m = 0 ; m < listNH.size() ; m++){
//                            Map map3 = (Map) listNH.get(m);
//                            dates.add((String) map3.get("观测时间"));
//                            gcNHValue = gcNHValue + (double)map3.get("观测值");
//                        }
//                        //--------------------------------接口三专用
//                        Iterator date = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag3 = false;
//                            boolean flag4 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < listCOD.size() ; k++){
//                                Map map = (Map) listCOD.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < listNH.size() ; g++){
//                                    Map map2 = (Map) listNH.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=20){
//                                    flag3 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=1){
//                                    flag4 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=20){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=1){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //--------------------------------
//                        //当前水功能区ID相对应的COD指标项平均值
//                        if(listCOD.size()>0){
//                            avgGcCODValue = gcCODValue/listCOD.size();
//                        }
//                        //当前水功能区ID相对应的NH指标项平均值
//                        if(listNH.size()>0){
//                            avgGcNHValue = gcNHValue/listNH.size();
//                        }
//                        //如果listCOD和listNH都有
//                        if(listCOD.size()>0&&listNH.size()>0){
//                            if(avgGcCODValue <= 20){
//                                flag1 = true;
//                            }
//                            if(avgGcNHValue <= 1){
//                                flag2 = true;
//                            }
//                            if(flag1&&flag2){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //listCOD
//                        if(listCOD.size()>0&&listNH.size()==0){
//                            if(avgGcCODValue <= 20){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //listNH
//                        if(listNH.size()>0&&listCOD.size()==0){
//                            if(avgGcNHValue <= 1){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                    }else {
//                        //监测次数小于6次，水质指标是I类的，求平均观测值和I类水质比较
//                        double gcCODValue = 0;
//                        double avgGcCODValue = 0;
//                        double gcNHValue = 0;
//                        double avgGcNHValue = 0;
//                        boolean flag1 = false;
//                        boolean flag2 = false;
//                        Set<String> dates = new HashSet<String>();
//                        //存放I类水质(COD)
//                        List firstCODList = new ArrayList();
//                        //存放I类水质(NH)
//                        List firstNHList = new ArrayList();
//                        //存放不是I类水质(COD)
//                        List notFirstCODList = new ArrayList();
//                        //存放不是I类水质(NH)
//                        List notFirstNHList = new ArrayList();
//                        for(int m = 0 ; m < listCOD.size() ; m++){
//                            Map map3 = (Map) listCOD.get(m);
//                            if("1".equals((String)map3.get("水质目标"))){
//                                dates.add((String) map3.get("观测时间"));
//                                gcCODValue = gcCODValue + (double)map3.get("观测值");
//                                firstCODList.add(map3);
//                            }else{
//                                notFirstCODList.add(map3);
//                            }
//                        }
//                        for(int m = 0 ; m < listNH.size() ; m++){
//                            Map map3 = (Map) listNH.get(m);
//                            if("1".equals((String)map3.get("水质目标"))){
//                                dates.add((String) map3.get("观测时间"));
//                                gcNHValue = gcNHValue + (double)map3.get("观测值");
//                                firstNHList.add(map3);
//                            }else{
//                                notFirstNHList.add(map3);
//                            }
//                        }
//
//                        if(firstCODList.size()>0){
//                            avgGcCODValue = gcCODValue/firstCODList.size();
//                        }
//                        if(firstNHList.size()>0){
//                            avgGcNHValue = gcNHValue/firstNHList.size();
//                        }
//                        //----------------------接口三专用
//                        Iterator date = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag3 = false;
//                            boolean flag4 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < firstCODList.size() ; k++){
//                                Map map = (Map) firstCODList.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < firstNHList.size() ; g++){
//                                    Map map2 = (Map) firstNHList.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=15){
//                                    flag3 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=0.15){
//                                    flag4 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=15){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=0.15){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //----------------------
//                        //如果firstCODList和firstNHList都有
//                        if(firstCODList.size()>0&&firstNHList.size()>0){
//                            if(avgGcCODValue <= 15){
//                                flag1 = true;
//                            }
//                            if(avgGcNHValue <= 0.15){
//                                flag2 = true;
//                            }
//                            if(flag1&&flag2){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //firstCODList
//                        if(firstCODList.size()>0&&firstNHList.size()==0){
//                            if(avgGcCODValue <= 15){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //firstNHList
//                        if(firstCODList.size()==0&&firstNHList.size()>0){
//                            if(avgGcNHValue <= 0.15){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //---------------------------------------------------------------------
//                        //不是I类水质的，每次监测的观测值和III类水质比较
//                        double num1 = 0;
//                        double num2 = 0;
//                        boolean flag3 = false;
//                        boolean flag4 = false;
//                        Set<String> dates1 = new HashSet<String>();
//                        for(int m = 0 ; m < notFirstCODList.size();m++){
//                            Map map3 = (Map) notFirstCODList.get(m);
//                            dates1.add((String) map3.get("观测时间"));
//                            if((double)map3.get("观测值")<=20){
//                                num1++;
//                            }
//                        }
//                        for(int m = 0 ; m < notFirstNHList.size();m++){
//                            Map map3 = (Map) notFirstNHList.get(m);
//                            dates1.add((String) map3.get("观测时间"));
//                            if((double)map3.get("观测值")<=1){
//                                num2++;
//                            }
//                        }
//                        //---------------------------------接口三专用
//                        Iterator date1 = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag5 = false;
//                            boolean flag6 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < firstCODList.size() ; k++){
//                                Map map = (Map) firstCODList.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < firstNHList.size() ; g++){
//                                    Map map2 = (Map) firstNHList.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=20){
//                                    flag5 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=1){
//                                    flag6 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=20){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=1){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //-------------------------------------
//                        //如果notFirstCODList和notFirstNHList都有
//                        if(notFirstCODList.size()>0&&notFirstNHList.size()>0){
//                            if(num1/notFirstCODList.size()>0.8){
//                                flag3 = true;
//                            }
//                            if(num2/notFirstNHList.size()>0.8){
//                                flag4 = true;
//                            }
//                            if(flag3&&flag4){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //notFirstCODList
//                        if(notFirstCODList.size()>0&&notFirstNHList.size()==0){
//                            if(num1/notFirstCODList.size()>0.8){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained","是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //notFirstNHList
//                        if(notFirstCODList.size()==0&&notFirstNHList.size()>0){
//                            if(num2/notFirstNHList.size()>0.8){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                    }
//                    DecimalFormat df  = new DecimalFormat("######0.00");
////                    waterFounctionMap.put("达标次数",countDB);
////                    waterFounctionMap.put("监测次数",count);
//                    waterFounctionMap.put("attainedPercent",df.format(countDB/count*100)+"%");
//                    waterFounctionList.add(waterFounctionMap);
//                }
//                double upLenth = 0;
//                double totalUpLength = 0;
//                String position = null;
//                //该行政区的达标水功能区的上游长度
//                for (int num = 0; num < waterFunctionIDs.size(); num++) {
//                    for (int x = 0; x < xzqList.size(); x++) {
//                        Map map3 = (Map) xzqList.get(x);
//                        if (waterFunctionIDs.get(num).equals(map3.get("水功能区_ID"))) {
//                            BigDecimal length = (BigDecimal) map3.get("上游长度");
//                            upLenth = upLenth + length.doubleValue();
//                        }
//                    }
//                }
//                //该行政区的水功能区上游总长(有观测值的水功能区)
//                for(int num = 0 ; num < totalDmList.size() ; num++){
//
//                    List list = (List) totalDmList.get(num);
////                    String id = null;
//                    for(int y = 0 ; y < list.size() ; y++){
//                        Map map = (Map) list.get(y);
//                        if(position == null){
//                            position = (String) map.get("水功能区经纬度");
//                        }
////                        if(id == null){
////                            id = (String) map.get("水功能区ID");
////                        }else{
////                            break;
////                        }
//                    }
//                    for (int x = 0; x < xzqList.size(); x++) {
//                        Map map3 = (Map) xzqList.get(x);
////                        if(id.equals(map3.get("水功能区_ID"))){
//                            BigDecimal length2 = (BigDecimal) map3.get("上游长度");
//                            totalUpLength = totalUpLength + length2.doubleValue();
////                        }
//                    }
//                }
//
//                DecimalFormat df  = new DecimalFormat("######0.00");
//                double size1 = waterFunctionIDs.size();
//                double size2 = xzqList.size();//totalDmList.size();
//                double size = size1/size2*100;
//                reachMap = new LinkedHashMap<String, Object>();
////                reachMap.put("行政区划名称", name);
//                reachMap.put("totalCount",size2);
//                reachMap.put("attainedCount",size1);
//
//                reachMap.put("个数达标率", df.format(size)  + "%");
//                reachMap.put("attainedPercent", df.format(size)  + "%");
//                reachMap.put("totalLength",totalUpLength);
//                reachMap.put("attainedLength",upLenth);
//
//                reachMap.put("长度达标率", df.format(upLenth / totalUpLength * 100) + "%");
//                reachMap.put("lengthPercent", df.format(upLenth / totalUpLength * 100) + "%");
//                reachMap.put("x",position.split(",")[0]);
//                reachMap.put("y",position.split(",")[1]);
//                reachMap.put("水功能区详细信息",waterFounctionList);
////                list.add(reachMap);
//            }
//
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                if(rs != null)
//                    rs.close();
//                if(ps != null)
//                    ps.close();
//                if(connection != null)
//                    connection.close();
//            } catch (SQLException e) {
//                e.printStackTrace();
//                throw e;
//            }
//        }
//        return reachMap;
//    }

    @Override
    public List getAllXzqWaterQuality(String water_body_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<String> xzqIDs = new ArrayList<String>();
        List allList = new ArrayList();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select distinct t2.上游行政区_id\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            ps.setString(2,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                xzqIDs.add(rs.getString("上游行政区_ID"));
            }
            for(int p = 0 ; p < xzqIDs.size() ; p++){
                List list = getPollution(water_body_id, xzqIDs.get(p), year);
                if (list.size()>0){
                    allList.add(list);
                }
            }
            for(int i = 0 ; i < allList.size() ; i++){
                List list = (List) allList.get(i);
                for(int j = 0 ; j < list.size() ; j++){
                    Map map = (Map) list.get(j);
                    map.remove("x");
                    map.remove("y");
                    map.remove("lengthPercent");
                    map.remove("attainedPercent");
                    map.remove("水功能区详细信息");
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null)
                    rs.close();
                if(ps != null)
                    ps.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
        return allList;
    }

    @Override
    public List<Map<String, Object>> getPollution(String water_body_id, String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
        //存放该行政区划范围内所有水功能区起止年份内化学需氧量和氨氮两个指标项的污染物入河量
        List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
        //存放标准定义为0的年份
        List<Integer> years = new ArrayList<Integer>();
        //存放查询出数据的年份
//        Set<String> selectYears = new HashSet<String>();
        //存放行政区的水质达标状况结果
        List resultList = new ArrayList();
        //根据传入的年份year，通过szpj_sys_污染物限排标准这个表找出计算水功能区污染物入河量的年份
        //存放计算水功能区污染物入河量的开始的年份
        int startYear = 2001;
        int endYear = Integer.parseInt(year);
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select distinct 年份 from szpj_sys_污染物限排标准 where 标准定义 = '0'";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            while (rs.next()){
                years.add(rs.getInt("年份"));
            }
            for(int i = 0 ; i < years.size() ; i++){
                if(endYear>years.get(i)){
                    startYear = years.get(i)+1;
                    break;
                }
            }
            rs.close();
            ps.close();
            String sql2 = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "union\n" +
                    "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n";
            ps = connection.prepareStatement(sql2);
            ps.setInt(1, startYear);
            ps.setInt(2, endYear);
            ps.setString(3, xzq_id);
            ps.setString(4,water_body_id);
            ps.setInt(5, startYear);
            ps.setInt(6, endYear);
            ps.setString(7, xzq_id);
            ps.setString(8,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                String selectYear = sdf.format(rs.getDate("监测日期"));
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                map.put("监测日期", selectYear);
//                selectYears.add(selectYear);
                list.add(map);
            }
            //将list分组，每个年份分为一组
//            Iterator selectYear = selectYears.iterator();
//            while (selectYear.hasNext()){
//                String currentYear = (String) selectYear.next();
                //存放当前年份的各个水功能区的两个污染物指标项的污染物入河量
//                List currentYearTotalItemValueList = new ArrayList();
//                for(int i = 0 ; i < list.size() ; i++){
//                    String date = (String) list.get(i).get("监测日期");
//                    if(currentYear.equals(date)){
//                        currentYearTotalItemValueList.add(list.get(i));
//                    }
//                }
                //将 currentYearTotalItemValueList 里边的化学需氧量和氨氮的入河量分别相加，和相对应的限排标准相比较
                String sql3 = "select t1.年份,t1.行政区划_ID,t2.sys_name,t1.年度累加限排总量,t1.水功能区数量最严格达标率,t1.水功能区长度最严格达标率 from szpj_sys_污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and 年份 = ?";
                ps = connection.prepareStatement(sql3);
                ps.setInt(1, Integer.parseInt(year));
                rs = ps.executeQuery();
                ResultSetMetaData metaData1 = rs.getMetaData();
                List<Map<String,Object>> xianPaiList = new ArrayList<Map<String,Object>>();
                while (rs.next()){
                    Map<String,Object> map = new HashMap<String,Object>();
                    for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                        String columnName = metaData1.getColumnName(i+1);
                        map.put(columnName,rs.getObject(columnName));
                    }
                    xianPaiList.add(map);
                }
                double valueCOD = 0;
                double valueNH = 0;
                double xianPaiCOD = 0;
                double xianPaiNH = 0;
                double numDB = 0;
                double lengthDB = 0;
                String xzqName = null;
                for(int i = 0 ; i < xianPaiList.size() ; i++){
                    Map map = xianPaiList.get(i);
                    if("化学需氧量(COD)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                        BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                        xianPaiCOD = value.doubleValue();
                        BigDecimal value1 = (BigDecimal) map.get("水功能区数量最严格达标率");
                        numDB = value1.doubleValue();
                        BigDecimal value2 = (BigDecimal) map.get("水功能区长度最严格达标率");
                        lengthDB = value2.doubleValue();
                    }
                    if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                        BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                        xianPaiNH = value.doubleValue();
                    }
                }
                for(int i = 0 ; i < list.size() ; i++){
                    Map map = (Map) list.get(i);
                    if(xzqName == null){
                        xzqName = (String) map.get("行政区划名称");
                    }
                    if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("污染物入河量");
                        valueCOD = valueCOD + value.doubleValue();
                    }
                    if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("污染物入河量");
                        valueNH = valueNH + value.doubleValue();
                    }
                }
                Map tempMap = new LinkedHashMap();
                List<Map<Object,Object>> xzqList = getxzqList(water_body_id,xzq_id);
                Map map = getSzyWaterQuality(xzqList,water_body_id,xzq_id,year);
                DecimalFormat df  = new DecimalFormat("######0.##");
                if(map != null && map.size()>0){
                    if(xianPaiCOD>0){
                        String str1 = (String) map.get("个数达标率");
                        String str2 = (String) map.get("长度达标率");
                        double num = Double.parseDouble(str1.substring(0, str1.indexOf("%")));
                        double length = Double.parseDouble(str2.substring(0, str2.indexOf("%")));
                        tempMap.put("行政区划名称",xzqName);
                        tempMap.put("年份",year);
                        tempMap.put("化学需氧量(COD)",df.format(valueCOD));
                        tempMap.put("化学需氧量(COD)限排",df.format(xianPaiCOD));
                        tempMap.put("氨氮(NH3-N)",df.format(valueNH));
                        tempMap.put("氨氮(NH3-N)限排",df.format(xianPaiNH));
                        tempMap.putAll(map);
                        tempMap.put("水功能区数量最严格达标率", df.format(numDB)+"%");
                        tempMap.put("水功能区长度最严格达标率",df.format(lengthDB)+"%");
                        if(valueCOD<xianPaiCOD&&valueNH<xianPaiNH&&num<numDB&&length<lengthDB){
                            tempMap.put("水质达标状况","不超载");
                        }else if(valueCOD>xianPaiCOD||valueNH>xianPaiNH||(num>numDB&&length>lengthDB)){
                            tempMap.put("水质达标状况","超载区");
                        }else{
                            tempMap.put("水质达标状况","临界超载");
                        }
                        resultList.add(tempMap);
                    }
                }
//            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<String, String>> getSZYYearList(String water_body_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
        //最后结果
        List<Map<String,String>> list = new ArrayList<Map<String,String>>();
        //存放COD和NH的年份
        Set<String> set1 = new HashSet<String>();
        //存放能计算水质达标率的年份
        Set<String> set2 = new HashSet<String>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t1.监测日期\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       mgmb_rel_水功能区与水体     t7\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "union\n" +
                    "select t1.监测日期\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       mgmb_rel_水功能区与水体     t7\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,water_body_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                set1.add(sdf.format(rs.getDate("监测日期")));
            }
            rs.close();
            ps.close();
            String sql2 = "select t4.观测时间\n" +
                    "  from mgmb_水功能区信息简表     t1,\n" +
                    "       mgmb_河道断面信息简表     t2,\n" +
                    "       mgmb_rel_水功能区与水体   t3,\n" +
                    "       szpj_biz_水质指标观测数据 t4\n" +
                    " where t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.水功能区_id = t3.水功能区_id\n" +
                    "   and t4.观测点_id = t2.河道断面_id\n" +
                    "   and t3.水体_id = ?\n" +
                    "union\n" +
                    "select t4.观测时间\n" +
                    "  from mgmb_水功能区信息简表     t1,\n" +
                    "       mgmb_河道断面信息简表     t2,\n" +
                    "       mgmb_rel_水功能区与水体   t3,\n" +
                    "       szpj_biz_水质指标观测数据 t4\n" +
                    " where t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.水功能区_id = t3.水功能区_id\n" +
                    "   and t4.观测点_id = t2.河道断面_id\n" +
                    "   and t3.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)";
            ps = connection.prepareStatement(sql2);
            ps.setString(1, water_body_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                set2.add(sdf.format(rs.getDate("观测时间")));
            }
            set1.retainAll(set2);
            Iterator set = set1.iterator();
            while (set.hasNext()){
                Map<String,String> map = new HashMap<String, String>();
                String year = (String) set.next();
                map.put("year", year);
                list.add(map);
            }

        } catch (SQLException e) {
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
        return list;
    }

    @Override
    public List<Map<String, String>> getXZQDataList(String water_body_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List list = getAllXzqWaterQuality(water_body_id,year);
        List<Map<String,String>> dataList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            Set<String> remainXZQ = new HashSet<String>();
            Set<String> allXZQ = new HashSet<String>();
            Set<String> resultXZQ = new HashSet<String>();
            String sql = "select t3.行政区划名称\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "union\n" +
                    "select t3.行政区划名称\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id = t3.行政区_id";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            ps.setString(2,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                allXZQ.add(rs.getString(1));
            }
            rs.close();
            ps.close();
            if(list.size()>0){
                for(int i = 0 ; i < list.size() ; i++){
                    List list2 = (List) list.get(i);
                    for(int j = 0 ; j < list2.size() ; j++){
                        Map map = (Map) list2.get(j);
                        resultXZQ.add((String) map.get("行政区划名称"));
                        Map<String,String> dataMap = new LinkedHashMap<String,String>();
                        String sql1 = "select 行政区_ID,WR_SD_CODE from v_行政区划 where 行政区划名称 = ?";
                        ps = connection.prepareStatement(sql1);
                        ps.setString(1, (String) map.get("行政区划名称"));
                        rs = ps.executeQuery();
                        if (rs.next()){
                            dataMap.put("行政区_id",rs.getString("行政区_ID"));
                            dataMap.put("SUPER_OBJ_ID",rs.getString("WR_SD_CODE"));
                            if("不超载".equals(map.get("水质达标状况"))){
                                dataMap.put("承载力类别等级","0");
                                dataMap.put("达标颜色","#0x0000FF");
                            }
                            if("临界超载".equals(map.get("水质达标状况"))){
                                dataMap.put("承载力类别等级","1");
                                dataMap.put("达标颜色","#FCD209");
                            }
                            if("超载区".equals(map.get("水质达标状况"))){
                                dataMap.put("承载力类别等级","2");
                                dataMap.put("达标颜色","#FF0000");
                            }
                            dataList.add(dataMap);
                        }
                    }
                }
            }
            remainXZQ.addAll(allXZQ);
            remainXZQ.removeAll(resultXZQ);
            Iterator XZQName = remainXZQ.iterator();
            while (XZQName.hasNext()){
                String currentName = (String) XZQName.next();
                String sql2 = "select 行政区_ID,WR_SD_CODE from v_行政区划 where 行政区划名称 = ?";
                ps = connection.prepareStatement(sql2);
                ps.setString(1,currentName);
                rs = ps.executeQuery();
                if (rs.next()){
                    Map<String,String> dataMap = new LinkedHashMap<String,String>();
                    dataMap.put("行政区_id",rs.getString("行政区_ID"));
                    dataMap.put("SUPER_OBJ_ID",rs.getString("WR_SD_CODE"));
                    dataMap.put("承载力类别等级","");
                    dataMap.put("达标颜色","#FFFFFF");
                    dataList.add(dataMap);
                }
            }
        } catch (SQLException e) {
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
        return dataList;
    }
    public List<Map<Object,Object>> getOnexzqList(String water_body_id, String xzq_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放水功能区，控制断面，行政区所有信息
        List<Map<Object,Object>> xzqList = new ArrayList<Map<Object,Object>>();
        //存放水功能区，控制断面，行政区单条信息
        Map<Object,Object> xzqMap = null;
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t1.水功能区_id,\n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n" +
                    "       t4.WFZ_NM\n" +
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id = ?\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, xzq_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                xzqMap = new HashMap<Object,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    xzqMap.put(columnName,rs.getObject(columnName));
                }
                xzqList.add(xzqMap);
            }
        } catch (SQLException e) {
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

        return xzqList;
    }
    //water_body_id为单一河流ID
    public List<Map<Object, Object>> getXZQPollutionList(String water_body_id, String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
        //存放该行政区划范围内所有水功能区起止年份内化学需氧量和氨氮两个指标项的污染物入河量
        List<Map<Object,Object>> list = new ArrayList<Map<Object,Object>>();
        //存放标准定义为0的年份
        List<Integer> years = new ArrayList<Integer>();
        //存放查询出数据的年份
//        Set<String> selectYears = new HashSet<String>();
        //存放行政区的水质达标状况结果
        List resultList = new ArrayList();
        //根据传入的年份year，通过szpj_sys_污染物限排标准这个表找出计算水功能区污染物入河量的年份
        //存放计算水功能区污染物入河量的开始的年份
        int startYear = 2001;
        int endYear = Integer.parseInt(year);
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select distinct 年份 from szpj_sys_污染物限排标准 where 标准定义 = '0'";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            while (rs.next()){
                years.add(rs.getInt("年份"));
            }
            for(int i = 0 ; i < years.size() ; i++){
                if(endYear>years.get(i)){
                    startYear = years.get(i)+1;
                    break;
                }
            }
            rs.close();
            ps.close();
            String sql2 = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n";
            ps = connection.prepareStatement(sql2);
            ps.setInt(1, startYear);
            ps.setInt(2, endYear);
            ps.setString(3, xzq_id);
            ps.setString(4,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                String selectYear = sdf.format(rs.getDate("监测日期"));
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                map.put("监测日期", selectYear);
//                selectYears.add(selectYear);
                list.add(map);
            }
            //将list分组，每个年份分为一组
//            Iterator selectYear = selectYears.iterator();
//            while (selectYear.hasNext()){
//                String currentYear = (String) selectYear.next();
                //存放当前年份的各个水功能区的两个污染物指标项的污染物入河量
//                List currentYearTotalItemValueList = new ArrayList();
//                for(int i = 0 ; i < list.size() ; i++){
//                    String date = (String) list.get(i).get("监测日期");
//                    if(currentYear.equals(date)){
//                        currentYearTotalItemValueList.add(list.get(i));
//                    }
//                }
                //将 currentYearTotalItemValueList 里边的化学需氧量和氨氮的入河量分别相加，和相对应的限排标准相比较
                String sql3 = "select t1.年份,t1.行政区划_ID,t2.sys_name,t1.年度累加限排总量,t1.水功能区数量最严格达标率,t1.水功能区长度最严格达标率 from szpj_sys_污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and 年份 = ?";
                ps = connection.prepareStatement(sql3);
                ps.setInt(1, Integer.parseInt(year));
                rs = ps.executeQuery();
                ResultSetMetaData metaData1 = rs.getMetaData();
                List<Map<String,Object>> xianPaiList = new ArrayList<Map<String,Object>>();
                while (rs.next()){
                    Map<String,Object> map = new HashMap<String,Object>();
                    for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                        String columnName = metaData1.getColumnName(i+1);
                        map.put(columnName,rs.getObject(columnName));
                    }
                    xianPaiList.add(map);
                }
                double valueCOD = 0;
                double valueNH = 0;
                double xianPaiCOD = 0;
                double xianPaiNH = 0;
                double numDB = 0;
                double lengthDB = 0;
                String xzqName = null;
                for(int i = 0 ; i < xianPaiList.size() ; i++){
                    Map map = xianPaiList.get(i);
                    if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                        xianPaiCOD = value.doubleValue();
                        BigDecimal value1 = (BigDecimal) map.get("水功能区数量最严格达标率");
                        numDB = value1.doubleValue();
                        BigDecimal value2 = (BigDecimal) map.get("水功能区长度最严格达标率");
                        lengthDB = value2.doubleValue();
                    }
                    if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                        xianPaiNH = value.doubleValue();
                    }
                }
                for(int i = 0 ; i < list.size() ; i++){
                    Map map = (Map) list.get(i);
                    if(xzqName == null){
                        xzqName = (String) map.get("行政区划名称");
                    }
                    if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("污染物入河量");
                        valueCOD = valueCOD + value.doubleValue();
                    }
                    if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                        BigDecimal value = (BigDecimal) map.get("污染物入河量");
                        valueNH = valueNH + value.doubleValue();
                    }
                }
                Map tempMap = new LinkedHashMap();
                List<Map<Object,Object>> xzqList = getOnexzqList(water_body_id, xzq_id);
                Map map = getSzyWaterQuality(xzqList,water_body_id,xzq_id,year);
                DecimalFormat df  = new DecimalFormat("######0.00");
                if(map != null && map.size() > 0){
                    String str1 = (String) map.get("个数达标率");
                    String str2 = (String) map.get("长度达标率");
                    double num = Double.parseDouble(str1.substring(0, str1.indexOf("%")));
                    double length = Double.parseDouble(str2.substring(0, str2.indexOf("%")));
                    tempMap.put("行政区划名称",xzqName);
                    tempMap.put("年份",year);
                    tempMap.put("化学需氧量(COD)",valueCOD);
                    tempMap.put("化学需氧量(COD)限排",xianPaiCOD);
                    tempMap.put("氨氮(NH3-N)",valueNH);
                    tempMap.put("氨氮(NH3-N)限排",xianPaiNH);
                    tempMap.putAll(map);
                    tempMap.put("水功能区数量最严格达标率", df.format(numDB)+"%");
                    tempMap.put("水功能区长度最严格达标率",df.format(lengthDB)+"%");
                    if(valueCOD<xianPaiCOD&&valueNH<xianPaiNH&&num<numDB&&length<lengthDB){
                        tempMap.put("水质达标状况","不超载");
                    }else if(valueCOD>xianPaiCOD||valueNH>xianPaiNH||(num>numDB&&length>lengthDB)){
                        tempMap.put("水质达标状况","超载区");
                    }else{
                        tempMap.put("水质达标状况","临界超载");
                    }
                    resultList.add(tempMap);
                }
//            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<String, Object>> getXZQCondition(String water_body_id, String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Object,Object>> list1 = new ArrayList<Map<Object, Object>>();
        List list2 = new ArrayList();

        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t6.水体_id,\n" +
                    "       t6.水体名称,\n" +
                    "       t1.水功能区_id,\n" +
                    "       \n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n"+
                    "       t4.WFZ_NM\n"+
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5,\n" +
                    "       v_水体                  t6\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id = ?\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n" +
                    "   and t5.水体_id = t6.水体_id\n" +
                    "union\n" +
                    "select t6.水体_id,\n" +
                    "       t6.水体名称,\n" +
                    "       t1.水功能区_id,\n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n"+
                    "       t4.WFZ_NM\n"+
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5,\n" +
                    "       v_水体                  t6\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n" +
                    "   and t5.水体_id = t6.水体_id";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,xzq_id);
            ps.setString(2, water_body_id);
            ps.setString(3,xzq_id);
            ps.setString(4, water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<Object,Object> map = new HashMap<Object,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                list1.add(map);
            }
            rs.close();
            ps.close();



            Set<String> riverNames = new HashSet<String>();
            for(int i = 0 ; i < list1.size() ; i++){
                riverNames.add((String) list1.get(i).get("水体名称"));
            }
            Iterator riverName = riverNames.iterator();
            while (riverName.hasNext()){
                String currentRiverName = (String) riverName.next();
                String currentRiverID = null;
                List<Map<Object,Object>> currentTotalList = new ArrayList<Map<Object, Object>>();
                for(int i = 0; i < list1.size() ; i++){
                    if(list1.get(i).get("水体名称").equals(currentRiverName)){
                        if(currentRiverID == null){
                            currentRiverID = (String) list1.get(i).get("水体_ID");
                        }
                        currentTotalList.add(list1.get(i));
                    }
                }
                List list3 = new ArrayList();
                String sql2 = "select t2.sys_name,t1.年度累加限排总量 from szpj_sys_河流污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and t1.年份 = ? and t1.行政区_id = ?  and t1.河流_id = ? ";
                ps = connection.prepareStatement(sql2);
                ps.setInt(1, Integer.valueOf(year));
                ps.setString(2, xzq_id);
                ps.setString(3,currentRiverID);
                rs = ps.executeQuery();
                ResultSetMetaData metaData1 = rs.getMetaData();
                while (rs.next()){
                    Map map = new HashMap();
                    for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                        String columnName = metaData1.getColumnName(i+1);
                        map.put(columnName,rs.getObject(columnName));
                    }
                    list3.add(map);
                }
                Map<String,Object> map = new LinkedHashMap<>();
                if(map != null){
                    map.put("code",currentRiverID);
                    map.put("name", currentRiverName);
                    Map m = getSzyWaterQuality(currentTotalList, water_body_id, xzq_id, year);
                    if(m!=null && m.size()>0){
                        map.putAll(m);
                    }
                    List<Map<Object,Object>> list = getXZQPollutionList(currentRiverID, xzq_id, year);
                    if(list.size()>0){
                        for(int i = 0 ; i < list.size() ; i++){
                            if(list.get(i).get("年份").equals(year)){
                                map.put("cod",list.get(i).get("化学需氧量(COD)"));
                                map.put("inputValue",list.get(i).get("氨氮(NH3-N)"));
                            }
                        }
                        for(int i = 0 ; i < list3.size() ; i++){
                            Map map1 = (Map) list3.get(i);
                            if(map1.get("SYS_NAME").equals("化学需氧量(COD)")){
                                map.put("limitCod",map1.get("年度累加限排总量"));
                            }
                            if(map1.get("SYS_NAME").equals("氨氮(NH3-N)")){
                                map.put("limitValue",map1.get("年度累加限排总量"));
                            }
                        }
                        list2.add(map);
                    }
                }
            }
            Set<String> remainRiverNames = new HashSet<String>();
            Set<String> resultRiverNames = new HashSet<String>();
            for(int i = 0 ; i < list2.size() ; i++){
                Map m = (Map) list2.get(i);
                resultRiverNames.add((String) m.get("name"));
                m.remove("长度达标率");
                m.remove("个数达标率");
                m.remove("水功能区详细信息");
            }
            remainRiverNames.addAll(riverNames);
            remainRiverNames.removeAll(resultRiverNames);
            if(remainRiverNames.size()>0){
                Iterator name = remainRiverNames.iterator();
                String sql2 = "select t2.sys_name,t1.年度累加限排总量 from szpj_sys_河流污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and t1.年份 = ? and t1.行政区_id = ?  and t1.河流_id = ? ";
                ps = connection.prepareStatement(sql2);

                while (name.hasNext()){
                    Map noResult = new LinkedHashMap();
                    List list3 = new ArrayList();
                    int waterFoucCount = 0;
                    double totalUpLength = 0;
                    String currentName = (String) name.next();
                    String currentId = null;
                    String x = null;
                    String y = null;
                    for(int i = 0 ; i < list1.size() ; i++){
                        Map currentMap = list1.get(i);
                        if(currentName.equals(currentMap.get("水体名称"))){
                            waterFoucCount++;
                            BigDecimal length2 = (BigDecimal) currentMap.get("上游长度");
                            totalUpLength = totalUpLength + length2.doubleValue();
                            if(currentId == null){
                                currentId = (String) currentMap.get("水体_ID");
                            }
                            if(x == null){
                                x = ((String) currentMap.get("经纬度坐标")).split(",")[0];
                            }
                            if(y == null){
                                y = ((String) currentMap.get("经纬度坐标")).split(",")[1];
                            }
                        }
                    }
                    noResult.put("code",currentId);
                    noResult.put("name",currentName);
                    noResult.put("totalCount",waterFoucCount);
                    noResult.put("attainedCount",0);
                    noResult.put("attainedPercent",0+"%");
                    noResult.put("totalLength",totalUpLength);
                    noResult.put("attainedLength",0);
                    noResult.put("lengthPercent",0+"%");
                    noResult.put("x",x);
                    noResult.put("y",y);
                    noResult.put("cod",0);
                    noResult.put("inputValue",0);
                    ps.setInt(1, Integer.valueOf(year));
                    ps.setString(2, xzq_id);
                    ps.setString(3,currentId);
                    rs = ps.executeQuery();
                    ResultSetMetaData metaData1 = rs.getMetaData();
                    while (rs.next()){
                        Map map = new HashMap();
                        for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                            String columnName = metaData1.getColumnName(i+1);
                            map.put(columnName,rs.getObject(columnName));
                        }
                        list3.add(map);
                    }
                    for(int i = 0 ; i < list3.size() ; i++){
                        Map map1 = (Map) list3.get(i);
                        if(map1.get("SYS_NAME").equals("氨氮(NH3-N)")){
                            noResult.put("limitValue",map1.get("年度累加限排总量"));
                        }
                        if(map1.get("SYS_NAME").equals("化学需氧量(COD)")){
                            noResult.put("limitCod",map1.get("年度累加限排总量"));
                        }
                    }
                    list2.add(noResult);
                }
            }
        } catch (SQLException e) {
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
        return list2;
    }

//    public Map<String, Object> getSzyWaterQualityT(List<Map<Object,Object>> xzqList,String water_body_id , String xzq_id, String year) throws SQLException {
//        Connection connection = null;
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//        String startDate = year + "-01-01";
//        String endDate = year + "-12-31";
//        //存放所属行政区内的达标水功能区的个数达标率和长度达标率
////        List<Map<String,String>> list = new ArrayList<Map<String,String>>();
//        Map<String,Object> reachMap = new LinkedHashMap<String, Object>();
//
//        //存放水功能区，控制断面，行政区所有信息
////        List<Map<Object,Object>> xzqList = getxzqList(water_body_id,xzq_id);
//        //存放水功能区，控制断面，行政区单条信息
////        Map<Object,Object> xzqMap = null;
//
//        //存放所有控制断面的年度观测情况信息
//        List totalDmList = new ArrayList();
//        //存放每个控制断面年度观测情况信息
//        List<Map<Object,Object>> dmList = null;
//        //存放每个控制断面年度观测情况一条信息
//        Map<Object,Object> dmMap = null;
//
//        //存放所有重组控制断面的年度观测情况信息(化学需氧量)
//        List newTotalDmCODList = new ArrayList();
//        //存放每个重组控制断面的年度观测情况信息
//        List newDmCODList = null;
//        //存放每个重组控制断面年度观测情况一条信息
////        Map newDmCODMap = null;
//
//        //存放所有重组控制断面的年度观测情况信息(氨氮)
//        List newTotalDmNHList = new ArrayList();
//        //存放每个重组控制断面的年度观测情况信息
//        List newDmNHList = null;
//        //存放每个重组控制断面年度观测情况一条信息
////        Map newDmNHMap = null;
//        //开始进行水资源承载力之水功能区水质达标率
//        try {
//            //先通过行政区_ID找出对应的水功能区信息
//            connection = basicDataSource.getConnection();
//            //通过xzqList里边的河道断面_ID也就是水功能区里边的控制断面找到当前年份的氨氮(NH3-N)和化学需氧量(COD)的相关信息
//            String sql2 = "select t1.观测时间,t1.观测值,t2.sys_name from szpj_biz_水质指标观测数据 t1,szpj_sys_指标项 t2 where t1.观测item_id = t2.item_id and t2.sys_name in ('氨氮(NH3-N)','化学需氧量(COD)') and to_char(t1.观测时间,'yyyy-mm-dd') between ? and ? and t1.观测点_id = ?";
//            ps = connection.prepareStatement(sql2);
//            ps.setString(1,startDate);
//            ps.setString(2,endDate);
//            for(int size = 0 ; size < xzqList.size() ; size++){
//                dmList = new ArrayList<Map<Object,Object>>();
//                ps.setString(3, (String) xzqList.get(size).get("河道断面_ID"));
//                rs = ps.executeQuery();
//                ResultSetMetaData metaData2 = rs.getMetaData();
//                while (rs.next()){
//                    dmMap = new HashMap<Object,Object>();
//                    for(int i = 0 ; i < metaData2.getColumnCount() ; i++){
//                        String columnName = metaData2.getColumnName(i+1);
//                        dmMap.put(columnName,rs.getObject(columnName));
//                    }
//                    dmMap.put("水功能区ID",xzqList.get(size).get("水功能区_ID"));
//                    dmMap.put("水质目标",xzqList.get(size).get("水质目标"));
//                    dmMap.put("行政区划名称", xzqList.get(size).get("行政区划名称"));
//                    dmMap.put("水功能区经纬度",xzqList.get(size).get("经纬度坐标"));
//                    dmMap.put("水功能区名称",xzqList.get(size).get("WFZ_NM"));
////                    dmMap.put("上游长度",xzqList.get(size).get("上游长度"));
//                    dmList.add(dmMap);
//                }
//                if(dmList.size()>0){
//                    totalDmList.add(dmList);
//                }
//            }
//            Set<String> funId = new HashSet<String>();
//            Set<String> funId1 = new HashSet<String>();
//            Set<String> funId2 = new HashSet<String>();
//            for(int i = 0 ; i < totalDmList.size() ; i++){
//                List list = (List) totalDmList.get(i);
//                for(int j = 0 ; j < list.size() ; j++){
//                    Map map = (Map) list.get(j);
//                    funId1.add((String)map.get("水功能区ID"));
//                }
//            }
//            for(int size = 0 ; size < xzqList.size() ; size++){
//                Map map = xzqList.get(size);
//                funId2.add((String) map.get("水功能区_ID"));
//            }
//            funId.addAll(funId2);
//            funId.removeAll(funId1);
//            List waterFounctionList = new ArrayList();
//            if(funId.size() > 0){
//                Iterator ids = funId.iterator();
//                while (ids.hasNext()){
//                    String currentId = (String) ids.next();
//                    for(int k = 0 ; k < xzqList.size() ; k++){
//                        Map map = xzqList.get(k);
//                        Map waterFunc = new LinkedHashMap();
//                        if(currentId.equals( map.get("水功能区_ID"))){
//                            waterFunc.put("code",map.get("水体_ID"));
//                            waterFunc.put("name",map.get("水体名称"));
//                            waterFunc.put("funCode",map.get("水功能区_ID"));
//                            waterFunc.put("funName",map.get("WFZ_NM"));
//                            waterFunc.put("isAttained","");
//                            waterFunc.put("attainedPercent","0%");
//                            waterFunc.put("x",((String)map.get("经纬度坐标")).split(",")[0]);
//                            waterFunc.put("y",((String)map.get("经纬度坐标")).split(",")[1]);
//                            waterFounctionList.add(waterFunc);
//                        }
//                    }
//                }
//            }
//
//            //将totalDmList中的观测时间的时间戳格式改为年月日格式
//
//            if(totalDmList.size()>0) {
//                for (int i = 0; i < totalDmList.size(); i++) {
//                    List list1 = (List) totalDmList.get(i);
//                    for (int j = 0; j < list1.size(); j++) {
//                        Map map = (Map) list1.get(j);
//                        String date = sdf.format((java.util.Date) map.get("观测时间"));
//                        map.put("观测时间", date);
//                    }
//                }
//                //计算所有控制断面中每个控制断面的当前年份的监测次数
//                for (int i = 0; i < totalDmList.size(); i++) {
//                    Set dateSet = new HashSet();
//                    List list1 = (List) totalDmList.get(i);
//                    for (int j = 0; j < list1.size(); j++) {
//                        Map map = (Map) list1.get(j);
//                        dateSet.add(map.get("观测时间"));
//                    }
//                    //将观测时间相同的观测值做平均值
//                    if (dateSet.size() > 0) {
//                        Iterator dates = dateSet.iterator();
//                        newDmCODList = new ArrayList();
//                        newDmNHList = new ArrayList();
//                        while (dates.hasNext()) {
//                            //存放观测时间相同的断面监测指标项的信息
//                            List sameTimeList1 = new ArrayList();
//                            List sameTimeList2 = new ArrayList();
//                            String currentDate = (String) dates.next();
//                            for (int k = 0; k < list1.size(); k++) {
//                                Map map = (Map) list1.get(k);
//                                if (map.get("观测时间").equals(currentDate)) {
//                                    if (map.get("SYS_NAME").equals("化学需氧量(COD)")) {
//                                        sameTimeList1.add(map);
//                                    }
//                                    if (map.get("SYS_NAME").equals("氨氮(NH3-N)")) {
//                                        sameTimeList2.add(map);
//                                    }
//                                }
//                            }
//                            if (sameTimeList1.size() > 0) {
//                                double gcSumValue = 0;
//                                double gcAvgValue = 0;
//                                String waterID = null;
//                                String xzqName = null;
//                                String goal = null;
//                                String gcTime = null;
//                                String itemName = null;
//                                for (int m = 0; m < sameTimeList1.size(); m++) {
//                                    Map map = (Map) sameTimeList1.get(m);
//                                    if (StringUtils.isEmpty(waterID)) {
//                                        waterID = (String) map.get("水功能区ID");
//                                    }
//                                    if (StringUtils.isEmpty(xzqName)) {
//                                        xzqName = (String) map.get("行政区划名称");
//                                    }
//                                    if (StringUtils.isEmpty(goal)) {
//                                        goal = (String) map.get("水质目标");
//                                    }
//                                    if (StringUtils.isEmpty(gcTime)) {
//                                        gcTime = (String) map.get("观测时间");
//                                    }
//                                    if (StringUtils.isEmpty(itemName)) {
//                                        itemName = (String) map.get("SYS_NAME");
//                                    }
//                                    BigDecimal gcValue = (BigDecimal) map.get("观测值");
//                                    gcSumValue += gcValue.doubleValue();
//                                }
//                                gcAvgValue = gcSumValue / sameTimeList1.size();
//                                Map newDmCODMap = new LinkedHashMap();
//                                newDmCODMap.put("水功能区ID", waterID);
//                                newDmCODMap.put("行政区划名称", xzqName);
//                                newDmCODMap.put("水质目标", goal);
//                                newDmCODMap.put("观测时间", gcTime);
//                                newDmCODMap.put("指标项", itemName);
//                                newDmCODMap.put("观测值", gcAvgValue);
//                                newDmCODList.add(newDmCODMap);
//                            }
//                            if (sameTimeList2.size() > 0) {
//                                double gcSumValue = 0;
//                                double gcAvgValue = 0;
//                                String waterID = null;
//                                String xzqName = null;
//                                String goal = null;
//                                String gcTime = null;
//                                String itemName = null;
//                                for (int m = 0; m < sameTimeList2.size(); m++) {
//                                    Map map = (Map) sameTimeList2.get(m);
//                                    if (StringUtils.isEmpty(waterID)) {
//                                        waterID = (String) map.get("水功能区ID");
//                                    }
//                                    if (StringUtils.isEmpty(xzqName)) {
//                                        xzqName = (String) map.get("行政区划名称");
//                                    }
//                                    if (StringUtils.isEmpty(goal)) {
//                                        goal = (String) map.get("水质目标");
//                                    }
//                                    if (StringUtils.isEmpty(gcTime)) {
//                                        gcTime = (String) map.get("观测时间");
//                                    }
//                                    if (StringUtils.isEmpty(itemName)) {
//                                        itemName = (String) map.get("SYS_NAME");
//                                    }
//                                    BigDecimal gcValue = (BigDecimal) map.get("观测值");
//                                    gcSumValue += gcValue.doubleValue();
//                                }
//                                gcAvgValue = gcSumValue / sameTimeList2.size();
//                                Map newDmNHMap = new LinkedHashMap();
//                                newDmNHMap.put("水功能区ID", waterID);
//                                newDmNHMap.put("行政区划名称", xzqName);
//                                newDmNHMap.put("水质目标", goal);
//                                newDmNHMap.put("观测时间", gcTime);
//                                newDmNHMap.put("指标项", itemName);
//                                newDmNHMap.put("观测值", gcAvgValue);
//                                newDmNHList.add(newDmNHMap);
//                            }
//                        }
//                        if(newDmCODList!=null&&newDmCODList.size()>0){
//                            newTotalDmCODList.add(newDmCODList);
//                        }
//                        if(newDmNHList!=null&&newDmNHList.size()>0){
//                            newTotalDmNHList.add(newDmNHList);
//                        }
//                    }
//                }
//                //开始计算比较是否达标
//                //因为该行政区内的水功能区的控制断面的监测指标项不一定有两项，所以应该根据totalDmList里边包含的水功能区来分别查找
//                //newTotalDmCODList和newTotalDmNHList的大小，取最大的那个作为该水功能区的监测次数
//                //存放达标的水功能区ID
//                List<String> waterFunctionIDs = new ArrayList<String>();
//                String name = null;
//                //存放某一条河流某一个行政区内的所有水功能区信息
//
//                for(int i = 0 ; i < totalDmList.size() ; i++){
//                    List list1 = (List) totalDmList.get(i);
//                    //找到当前的水功能区ID
//                    String waterFounctionID = null;
//                    String waterFounctionName = null;
//                    //存放某一条河流某一个行政区内的某一个水功能区信息
//                    Map waterFounctionMap = new LinkedHashMap();
//                    for(int j = 0 ; j < list1.size() ; j++){
//                        Map map1 = (Map) list1.get(j);
//                        name = (String)map1.get("行政区划名称");
//                        waterFounctionID = (String) map1.get("水功能区ID");
//                        waterFounctionName = (String)map1.get("水功能区名称");
//                    }
//                    waterFounctionMap.put("funCode",waterFounctionID);
//                    waterFounctionMap.put("funName",waterFounctionName);
//                    //根据当前的水功能区ID查找newTotalDmCODList和newTotalDmNHList，确定监测次数
//                    //存放当前水功能区ID相对应的COD指标项的list
//                    List listCOD = new ArrayList();
//                    //存放当前水功能区ID相对应的NH指标项的list
//                    List listNH = new ArrayList();
//                    for(int j = 0 ; j < newTotalDmCODList.size(); j++){
//                        List list2 = (List) newTotalDmCODList.get(j);
//                        for(int k = 0 ; k < list2.size() ; k++){
//                            Map map2 = (Map) list2.get(k);
//                            if(waterFounctionID.equals((String)map2.get("水功能区ID"))){
//                                if(listCOD.size()==0){
//                                    listCOD = list2;
//                                }
//                            }else{
//                                break;
//                            }
//                        }
//                    }
//                    for(int j = 0 ; j < newTotalDmNHList.size(); j++){
//                        List list2 = (List) newTotalDmNHList.get(j);
//                        for(int k = 0 ; k < list2.size() ; k++){
//                            Map map2 = (Map) list2.get(k);
//                            if(waterFounctionID.equals((String)map2.get("水功能区ID"))){
//                                if(listNH.size()==0){
//                                    listNH = list2;
//                                }
//                            }else{
//                                break;
//                            }
//                        }
//                    }
//                    //比较listCOD和listNH的大小，取最大值count作为监测次数
//                    int count = 0;
//                    double countDB = 0;
//                    if(listCOD.size()>=listNH.size()){
//                        count = listCOD.size();
//                    }else{
//                        count = listNH.size();
//                    }
//                    //根据监测次数的不同来进行不同的达标的计算
//                    if(count>=6){
//                        //监测次数大于等于6次，平均观测值和III类水质比较
//                        double gcCODValue = 0;
//                        double avgGcCODValue = 0;
//                        double gcNHValue = 0;
//                        double avgGcNHValue = 0;
//                        boolean flag1 = false;
//                        boolean flag2 = false;
//                        Set<String> dates = new HashSet<String>();
//                        for(int m = 0 ; m < listCOD.size() ; m++){
//                            Map map3 = (Map) listCOD.get(m);
//                            dates.add((String) map3.get("观测时间"));
//                            gcCODValue = gcCODValue + (double)map3.get("观测值");
//                        }
//                        for(int m = 0 ; m < listNH.size() ; m++){
//                            Map map3 = (Map) listNH.get(m);
//                            dates.add((String) map3.get("观测时间"));
//                            gcNHValue = gcNHValue + (double)map3.get("观测值");
//                        }
//                        //--------------------------------接口三专用
//                        Iterator date = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag3 = false;
//                            boolean flag4 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < listCOD.size() ; k++){
//                                Map map = (Map) listCOD.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < listNH.size() ; g++){
//                                    Map map2 = (Map) listNH.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=20){
//                                    flag3 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=1){
//                                    flag4 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=20){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=1){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //--------------------------------
//                        //当前水功能区ID相对应的COD指标项平均值
//                        if(listCOD.size()>0){
//                            avgGcCODValue = gcCODValue/listCOD.size();
//                        }
//                        //当前水功能区ID相对应的NH指标项平均值
//                        if(listNH.size()>0){
//                            avgGcNHValue = gcNHValue/listNH.size();
//                        }
//                        //如果listCOD和listNH都有
//                        if(listCOD.size()>0&&listNH.size()>0){
//                            if(avgGcCODValue <= 20){
//                                flag1 = true;
//                            }
//                            if(avgGcNHValue <= 1){
//                                flag2 = true;
//                            }
//                            if(flag1&&flag2){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //listCOD
//                        if(listCOD.size()>0&&listNH.size()==0){
//                            if(avgGcCODValue <= 20){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //listNH
//                        if(listNH.size()>0&&listCOD.size()==0){
//                            if(avgGcNHValue <= 1){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                    }else {
//                        //监测次数小于6次，水质指标是I类的，求平均观测值和I类水质比较
//                        double gcCODValue = 0;
//                        double avgGcCODValue = 0;
//                        double gcNHValue = 0;
//                        double avgGcNHValue = 0;
//                        boolean flag1 = false;
//                        boolean flag2 = false;
//                        Set<String> dates = new HashSet<String>();
//                        //存放I类水质(COD)
//                        List firstCODList = new ArrayList();
//                        //存放I类水质(NH)
//                        List firstNHList = new ArrayList();
//                        //存放不是I类水质(COD)
//                        List notFirstCODList = new ArrayList();
//                        //存放不是I类水质(NH)
//                        List notFirstNHList = new ArrayList();
//                        for(int m = 0 ; m < listCOD.size() ; m++){
//                            Map map3 = (Map) listCOD.get(m);
//                            if("1".equals((String)map3.get("水质目标"))){
//                                dates.add((String) map3.get("观测时间"));
//                                gcCODValue = gcCODValue + (double)map3.get("观测值");
//                                firstCODList.add(map3);
//                            }else{
//                                notFirstCODList.add(map3);
//                            }
//                        }
//                        for(int m = 0 ; m < listNH.size() ; m++){
//                            Map map3 = (Map) listNH.get(m);
//                            if("1".equals((String)map3.get("水质目标"))){
//                                dates.add((String) map3.get("观测时间"));
//                                gcNHValue = gcNHValue + (double)map3.get("观测值");
//                                firstNHList.add(map3);
//                            }else{
//                                notFirstNHList.add(map3);
//                            }
//                        }
//
//                        if(firstCODList.size()>0){
//                            avgGcCODValue = gcCODValue/firstCODList.size();
//                        }
//                        if(firstNHList.size()>0){
//                            avgGcNHValue = gcNHValue/firstNHList.size();
//                        }
//                        //----------------------接口三专用
//                        Iterator date = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag3 = false;
//                            boolean flag4 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < firstCODList.size() ; k++){
//                                Map map = (Map) firstCODList.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < firstNHList.size() ; g++){
//                                    Map map2 = (Map) firstNHList.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=15){
//                                    flag3 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=0.15){
//                                    flag4 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=15){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=0.15){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //----------------------
//                        //如果firstCODList和firstNHList都有
//                        if(firstCODList.size()>0&&firstNHList.size()>0){
//                            if(avgGcCODValue <= 15){
//                                flag1 = true;
//                            }
//                            if(avgGcNHValue <= 0.15){
//                                flag2 = true;
//                            }
//                            if(flag1&&flag2){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained", "否");
//                            }
//                        }
//                        //firstCODList
//                        if(firstCODList.size()>0&&firstNHList.size()==0){
//                            if(avgGcCODValue <= 15){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //firstNHList
//                        if(firstCODList.size()==0&&firstNHList.size()>0){
//                            if(avgGcNHValue <= 0.15){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //---------------------------------------------------------------------
//                        //不是I类水质的，每次监测的观测值和III类水质比较
//                        double num1 = 0;
//                        double num2 = 0;
//                        boolean flag3 = false;
//                        boolean flag4 = false;
//                        Set<String> dates1 = new HashSet<String>();
//                        for(int m = 0 ; m < notFirstCODList.size();m++){
//                            Map map3 = (Map) notFirstCODList.get(m);
//                            dates1.add((String) map3.get("观测时间"));
//                            if((double)map3.get("观测值")<=20){
//                                num1++;
//                            }
//                        }
//                        for(int m = 0 ; m < notFirstNHList.size();m++){
//                            Map map3 = (Map) notFirstNHList.get(m);
//                            dates1.add((String) map3.get("观测时间"));
//                            if((double)map3.get("观测值")<=1){
//                                num2++;
//                            }
//                        }
//                        //---------------------------------接口三专用
//                        Iterator date1 = dates.iterator();
//                        while (date.hasNext()){
//                            String currentDate = (String) date.next();
//                            boolean flag5 = false;
//                            boolean flag6 = false;
//                            Map map1 = new HashMap();
//                            for(int k = 0 ; k < firstCODList.size() ; k++){
//                                Map map = (Map) firstCODList.get(k);
//                                if(map.get("观测时间").equals(currentDate)){
//                                    map1.put(map.get("指标项"),map.get("观测值"));
//                                }
//                                for(int g = 0 ; g < firstNHList.size() ; g++){
//                                    Map map2 = (Map) firstNHList.get(g);
//                                    if(map2.get("观测时间").equals(currentDate)){
//                                        map1.put(map2.get("指标项"),map2.get("观测值"));
//                                    }
//                                }
//                            }
//                            if(map1.size()==2){
//                                if((double)map1.get("化学需氧量(COD)")<=20){
//                                    flag5 = true;
//                                }
//                                if((double)map1.get("氨氮(NH3-N)")<=1){
//                                    flag6 = true;
//                                }
//                                if(flag3 && flag4){
//                                    countDB++;
//                                }
//                            }else{
//                                Set sets = map1.keySet();
//                                Iterator set = sets.iterator();
//                                if (set.hasNext()){
//                                    String currentKey = (String) set.next();
//                                    if(currentKey.equals("化学需氧量(COD)")){
//                                        if((double)map1.get("化学需氧量(COD)")<=20){
//                                            countDB++;
//                                        }
//                                    }
//                                    if(currentKey.equals("氨氮(NH3-N)")){
//                                        if((double)map1.get("氨氮(NH3-N)")<=1){
//                                            countDB++;
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                        //-------------------------------------
//                        //如果notFirstCODList和notFirstNHList都有
//                        if(notFirstCODList.size()>0&&notFirstNHList.size()>0){
//                            if(num1/notFirstCODList.size()>0.8){
//                                flag3 = true;
//                            }
//                            if(num2/notFirstNHList.size()>0.8){
//                                flag4 = true;
//                            }
//                            if(flag3&&flag4){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //notFirstCODList
//                        if(notFirstCODList.size()>0&&notFirstNHList.size()==0){
//                            if(num1/notFirstCODList.size()>0.8){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained","是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                        //notFirstNHList
//                        if(notFirstCODList.size()==0&&notFirstNHList.size()>0){
//                            if(num2/notFirstNHList.size()>0.8){
//                                waterFunctionIDs.add(waterFounctionID);
//                                waterFounctionMap.put("isAttained", "是");
//                            }else{
//                                waterFounctionMap.put("isAttained","否");
//                            }
//                        }
//                    }
//                    DecimalFormat df  = new DecimalFormat("######0.00");
////                    waterFounctionMap.put("达标次数",countDB);
////                    waterFounctionMap.put("监测次数",count);
//                    waterFounctionMap.put("attainedPercent",df.format(countDB/count*100)+"%");
//                    waterFounctionList.add(waterFounctionMap);
//                }
//                double upLenth = 0;
//                double totalUpLength = 0;
//                String position = null;
//                //该行政区的达标水功能区的上游长度
//                for (int num = 0; num < waterFunctionIDs.size(); num++) {
//                    for (int x = 0; x < xzqList.size(); x++) {
//                        Map map3 = (Map) xzqList.get(x);
//                        if (waterFunctionIDs.get(num).equals(map3.get("水功能区_ID"))) {
//                            BigDecimal length = (BigDecimal) map3.get("上游长度");
//                            upLenth = upLenth + length.doubleValue();
//                        }
//                    }
//                }
//                //该行政区的水功能区上游总长(有观测值的水功能区)
//                for(int num = 0 ; num < totalDmList.size() ; num++){
//
//                    List list = (List) totalDmList.get(num);
//                    String id = null;
//                    for(int y = 0 ; y < list.size() ; y++){
//                        Map map = (Map) list.get(y);
//                        if(position == null){
//                            position = (String) map.get("水功能区经纬度");
//                        }
//                        if(id == null){
//                            id = (String) map.get("水功能区ID");
//                        }else{
//                            break;
//                        }
//                    }
//                    for (int x = 0; x < xzqList.size(); x++) {
//                        Map map3 = (Map) xzqList.get(x);
//                        if(id.equals(map3.get("水功能区_ID"))){
//                            BigDecimal length2 = (BigDecimal) map3.get("上游长度");
//                            totalUpLength = totalUpLength + length2.doubleValue();
//                        }
//                    }
//                }
//
//                DecimalFormat df  = new DecimalFormat("######0.00");
//                double size1 = waterFunctionIDs.size();
//                double size2 = totalDmList.size();
//                double size = size1/size2*100;
//
////                reachMap.put("行政区划名称", name);
//                reachMap.put("totalCount",size2);
//                reachMap.put("attainedCount",size1);
//
//                reachMap.put("个数达标率", df.format(size)  + "%");
//                reachMap.put("attainedPercent", df.format(size)  + "%");
//                reachMap.put("totalLength",totalUpLength);
//                reachMap.put("attainedLength",upLenth);
//
//                reachMap.put("长度达标率", df.format(upLenth / totalUpLength * 100) + "%");
//                reachMap.put("lengthPercent", df.format(upLenth / totalUpLength * 100) + "%");
//                reachMap.put("x",position.split(",")[0]);
//                reachMap.put("y", position.split(",")[1]);
//                reachMap.put("水功能区详细信息",waterFounctionList);
////                list.add(reachMap);
//            }else{
//                reachMap.put("水功能区详细信息",waterFounctionList);
//            }
//
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                if(rs != null){
//                    rs.close();
//                }
//                if(ps != null){
//                    ps.close();
//                }
//                if(connection != null){
//                    connection.close();
//                }
//            } catch (SQLException e) {
//                e.printStackTrace();
//                throw e;
//            }
//        }
//        return reachMap;
//    }

    //water_body_id为河流id
    public List<Map<String, Object>> getOneXZQCondition(String water_body_id, String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Object,Object>> list1 = new ArrayList<Map<Object, Object>>();
        List list2 = new ArrayList();
        List list3 = new ArrayList();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t6.水体_id,\n" +
                    "       t6.水体名称,\n" +
                    "       t1.水功能区_id,\n" +
                    "       \n" +
                    "       t1.上游行政区_id,\n" +
                    "       t1.上游长度,\n" +
                    "       t2.河道断面_id,\n" +
                    "       t3.行政区划名称,\n" +
                    "       t4.wfz_wqt 水质目标,\n" +
                    "       t1.经纬度坐标,\n"+
                    "       t4.WFZ_NM\n"+
                    "  from mgmb_水功能区信息简表   t1,\n" +
                    "       mgmb_河道断面信息简表   t2,\n" +
                    "       v_行政区划              t3,\n" +
                    "       wr_wfz_b                t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5,\n" +
                    "       v_水体                  t6\n" +
                    " where t1.上游行政区_id = ?\n" +
                    "   and t1.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t5.水体_id = ?\n" +
                    "   and t5.水功能区_id = t1.水功能区_id\n" +
                    "   and t5.水体_id = t6.水体_id\n";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,xzq_id);
            ps.setString(2,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<Object,Object> map = new HashMap<Object,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                list1.add(map);
            }
            rs.close();
            ps.close();



            Set<String> riverNames = new HashSet<String>();
            for(int i = 0 ; i < list1.size() ; i++){
                riverNames.add((String) list1.get(i).get("水体名称"));
            }
            Iterator riverName = riverNames.iterator();
            while (riverName.hasNext()){
                String currentRiverName = (String) riverName.next();
                String currentRiverID = null;
                List<Map<Object,Object>> currentTotalList = new ArrayList<Map<Object, Object>>();
                for(int i = 0; i < list1.size() ; i++){
                    if(list1.get(i).get("水体名称").equals(currentRiverName)){
                        if(currentRiverID == null){
                            currentRiverID = (String) list1.get(i).get("水体_ID");
                        }
                        currentTotalList.add(list1.get(i));
                    }
                }

                String sql2 = "select t2.sys_name,t1.年度累加限排总量 from szpj_sys_河流污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and t1.年份 = ? and t1.行政区_id = ?  and t1.河流_id = ? ";
                ps = connection.prepareStatement(sql2);
                ps.setInt(1, Integer.valueOf(year));
                ps.setString(2, xzq_id);
                ps.setString(3,currentRiverID);
                rs = ps.executeQuery();
                ResultSetMetaData metaData1 = rs.getMetaData();
                while (rs.next()){
                    Map map = new HashMap();
                    for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                        String columnName = metaData1.getColumnName(i+1);
                        map.put(columnName,rs.getObject(columnName));
                    }
                    list3.add(map);
                }

                Map<String,Object> map = new LinkedHashMap<>();
                if(map != null){
                    map.put("code",currentRiverID);
                    map.put("name",currentRiverName);
                    Map m = getSzyWaterQualityT(currentTotalList, water_body_id, xzq_id, year);
                    if(m!=null && m.size()>0){
                        map.putAll(m);
                    }
                    List<Map<Object,Object>> list = getXZQPollutionList(currentRiverID, xzq_id, year);
                    if(list.size()>0){
                        for(int i = 0 ; i < list.size() ; i++){
                            if(list.get(i).get("年份").equals(year)){
                                map.put("cod",list.get(i).get("化学需氧量(COD)"));
                                map.put("inputValue",list.get(i).get("氨氮(NH3-N)"));
                            }
                        }
                        for(int i = 0 ; i < list3.size() ; i++){
                            Map map1 = (Map) list3.get(i);
                            if(map1.get("SYS_NAME").equals("化学需氧量(COD)")){
                                map.put("limitCod",map1.get("年度累加限排总量"));
                            }
                            if(map1.get("SYS_NAME").equals("氨氮(NH3-N)")){
                                map.put("limitValue",map1.get("年度累加限排总量"));
                            }
                        }
                        list2.add(map);
                    }else{
                        list2.add(map);
                    }
                }
            }
            for(int i = 0 ; i < list2.size() ; i++){
                Map m = (Map) list2.get(i);
                m.remove("长度达标率");
                m.remove("个数达标率");
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return list2;
    }

    @Override
    public List<Map<String, String>> getSZYPWKObj(String waterBodyId) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETMONITOROBJ(?,?,?,?)}");
            call.setString(1, waterBodyId);
            call.setString(2, "排污口");
            call.setString(3,"");
            call.registerOutParameter(4, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(4);
            List<Map<String, String>> list = new ArrayList<Map<String, String>>();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("id", rs.getString("OBJ_ID"));
                map.put("name", rs.getString("名称"));
                map.put("location", rs.getString("CENTERXY"));
                map.put("mapId", rs.getString("MAP_ID"));
                map.put("superObjId", rs.getString("SUPER_OBJ_ID"));
                map.put("icoFlag", rs.getString("ICO_FLAG"));
                String str = "";
                map.put("typeIDs", rs.getString("SUPER_OBJ_ID"));
                if (rs.getString("MAIN_TYPEID") != null && !"".equals(rs.getString("MAIN_TYPEID"))) {
                    str += rs.getString("MAIN_TYPEID");
                }
                if (rs.getString("LEVEL_1_ID") != null && !"".equals(rs.getString("LEVEL_1_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_1_ID");
                }
                if (rs.getString("LEVEL_2_ID") != null && !"".equals(rs.getString("LEVEL_2_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_2_ID");
                }
                if (rs.getString("LEVEL_3_ID") != null && !"".equals(rs.getString("LEVEL_3_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_3_ID");
                }
                map.put("typeIDs",str);
                list.add(map);
            }
            rs.close();
            call.close();
            call = connection.prepareCall("{call SP_GETMONITOROBJ(?,?,?,?)}");
            call.setString(1, waterBodyId);
            call.setString(2, "控制断面");
            call.setString(3, "");
            call.registerOutParameter(4, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(4);
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("id", rs.getString("OBJ_ID"));
                map.put("name", rs.getString("名称"));
                map.put("location", rs.getString("CENTERXY"));
                map.put("mapId", rs.getString("MAP_ID"));
                map.put("superObjId", rs.getString("SUPER_OBJ_ID"));
                map.put("icoFlag", rs.getString("ICO_FLAG"));
                String str = "";
                map.put("typeIDs", rs.getString("SUPER_OBJ_ID"));
                if (rs.getString("MAIN_TYPEID") != null && !"".equals(rs.getString("MAIN_TYPEID"))) {
                    str += rs.getString("MAIN_TYPEID");
                }
                if (rs.getString("LEVEL_1_ID") != null && !"".equals(rs.getString("LEVEL_1_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_1_ID");
                }
                if (rs.getString("LEVEL_2_ID") != null && !"".equals(rs.getString("LEVEL_2_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_2_ID");
                }
                if (rs.getString("LEVEL_3_ID") != null && !"".equals(rs.getString("LEVEL_3_ID"))) {
                    if (!"".equals(str))
                        str += ",";
                    str += rs.getString("LEVEL_3_ID");
                }
                map.put("typeIDs",str);
                list.add(map);
            }
            return list;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null)
                    rs.close();
                if(call != null)
                    call.close();
                if(connection != null)
                    connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<Object, Object>> getWaterFounctionItemValue(String water_body_id, String year,String xzq_id, String riverID, String waterFounctionID) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Object,Object>> list = new ArrayList<Map<Object,Object>>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        int count = 0;
        double valueCOD = 0 ;
        double valueNH = 0;
        //存放结果的list
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object,Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select value from szpj_sys_指标达标值 where LEVEL_ID = '2' and item_id = '5F769D7EA7F54E509209E0B869301080'";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            if(rs.next()){
                valueCOD = rs.getDouble(1);
            }
            rs.close();
            ps.close();

            String sql2 = "select value from szpj_sys_指标达标值 where LEVEL_ID = '2' and item_id = '7207FF62066B4B18A80FF3A8B0FD479B'";
            ps = connection.prepareStatement(sql2);
            rs = ps.executeQuery();
            if(rs.next()){
                valueNH = rs.getDouble(1);
            }
            rs.close();
            ps.close();

            String sql3 = "select t1.水功能区_id,\n" +
                    "       t4.水体名称,\n" +
                    "       t5.wfz_nm,\n" +
                    "       svid.观测时间,\n" +
                    "       svid.化学需氧量,\n" +
                    "       svid.氨氮,\n" +
                    "       t1.经纬度坐标\n" +
                    "  from mgmb_水功能区信息简表 t1,\n" +
                    "       mgmb_河道断面信息简表 t2,\n" +
                    "       mgmb_rel_水功能区与水体 t3,\n" +
                    "       v_水体 t4,\n" +
                    "       wr_wfz_b t5,\n" +
                    "       (select vid.观测点_id,\n" +
                    "               vid.观测时间,\n" +
                    "               sum(vid.A) as 化学需氧量,\n" +
                    "               sum(vid.B) as 氨氮\n" +
                    "          from (select t.观测点_id, t.观测时间, t.观测值 as A, 0 as B\n" +
                    "                  from szpj_biz_水质指标观测数据 t\n" +
                    "                 where t.观测item_id = '5F769D7EA7F54E509209E0B869301080'\n" +
                    "                union\n" +
                    "                select t.观测点_id, t.观测时间, 0 as A, t.观测值 as B\n" +
                    "                  from szpj_biz_水质指标观测数据 t\n" +
                    "                 where t.观测item_id = '7207FF62066B4B18A80FF3A8B0FD479B') vid\n" +
                    "         GROUP BY vid.观测点_id, vid.观测时间) svid\n" +
                    " where t1.控制断面_cd = t2.wr_sd_code\n" +
                    "   and t1.水功能区_id = t3.水功能区_id\n" +
                    "   and t3.水体_id = t4.水体_id\n" +
                    "   and t1.wr_sd_code = t5.wfz_cd\n" +
                    "   and t2.河道断面_id = svid.观测点_id\n" +
                    "   and t1.上游行政区_id = ?\n" +
                    "   and t3.水体_id = ?\n" +
                    "   and t1.水功能区_id = ?\n" +
                    "   and to_char(svid.观测时间, 'yyyy') between ? and ?\n" +
                    " order by svid.观测时间\n";
            ps = connection.prepareStatement(sql3);
            ps.setString(1, xzq_id);
            ps.setString(2,riverID);
            ps.setString(3,waterFounctionID);
            ps.setString(4, year);
            ps.setString(5, year);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                count++;
                map.put("code", rs.getString("水功能区_ID"));
                map.put("name",rs.getString("WFZ_NM"));//rs.getString("水体名称")+
                map.put("monitorCount",count);
                map.put("monitorTime",sdf.format(rs.getTimestamp("观测时间")));
                map.put("codValue",rs.getDouble("化学需氧量"));
                map.put("codStandardValue",valueCOD);
                map.put("anValue",rs.getDouble("氨氮"));
                map.put("anStandardValue",valueNH);
                map.put("x",rs.getString("经纬度坐标").split(",")[0]);
                map.put("y",rs.getString("经纬度坐标").split(",")[1]);
                list.add(map);
            }
        } catch (SQLException e) {
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
        return list;
    }

    @Override
    public List<Map<Object, Object>> getPWKDetail(String water_body_id, String year, String xzq_id, String riverID, String waterFounctionID) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "SELECT t1.水功能区_id,\n" +
                    "       t4.wfz_nm,\n" +
                    "       svid.排污口_ID,\n" +
                    "       t3.pdo_nm,\n" +
                    "       svid.监测日期,\n" +
                    "       svid.A         化学需氧量,\n" +
                    "       svid.B         氨氮,\n" +
                    "       t2.经纬度坐标\n" +
                    "  FROM mgmb_水功能区信息简表 t1,\n" +
                    "       mgmb_入河排污口信息简表 t2,\n" +
                    "       wr_pdo_b t3,\n" +
                    "       wr_wfz_b t4,\n" +
                    "       mgmb_rel_水功能区与水体 t5,\n" +
                    "       (SELECT vid.排污口_ID, vid.监测日期, SUM(vid.A) AS A, SUM(vid.B) AS B\n" +
                    "          FROM (SELECT IV.排污口_ID,\n" +
                    "                       IV.监测日期,\n" +
                    "                       IV.污染物入河量 AS A,\n" +
                    "                       0               AS B\n" +
                    "                  FROM ZDWARNER.szpj_biz_排污口污染物入河量 IV\n" +
                    "                 WHERE IV.污染物指标_ID = '5F769D7EA7F54E509209E0B869301080'\n" +
                    "                UNION\n" +
                    "                SELECT IV.排污口_ID,\n" +
                    "                       IV.监测日期,\n" +
                    "                       0               AS A,\n" +
                    "                       IV.污染物入河量 AS B\n" +
                    "                  FROM ZDWARNER.szpj_biz_排污口污染物入河量 IV\n" +
                    "                 WHERE IV.污染物指标_ID = '7207FF62066B4B18A80FF3A8B0FD479B') vid\n" +
                    "         GROUP BY vid.排污口_ID, vid.监测日期) svid\n" +
                    " WHERE svid.排污口_ID = t2.入河排污口_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t4.wfz_cd = t3.wfz_cd\n" +
                    "   and t1.wr_sd_code = t4.wfz_cd\n" +
                    "   and t1.水功能区_id = t5.水功能区_id\n" +
                    "   and t5.水体_id = ?\n" +
                    "   and t1.上游行政区_id = ?\n" +
                    "   and t1.水功能区_id = ?\n" +
                    "   and to_char(svid.监测日期, 'yyyy') between ? and ?\n" +
                    " ORDER BY svid.监测日期 DESC, t3.pdo_nm\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,riverID);
            ps.setString(2,xzq_id);
            ps.setString(3,waterFounctionID);
            ps.setString(4,year);
            ps.setString(5,year);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                map.put("code",rs.getString("水功能区_ID"));
                map.put("name",rs.getString("WFZ_NM"));
                map.put("pollutionCode",rs.getString("排污口_ID"));
                map.put("pollutionName",rs.getString("PDO_NM"));
                map.put("cod",rs.getDouble("化学需氧量"));
                map.put("inputValue",rs.getDouble("氨氮"));
                map.put("x",rs.getString("经纬度坐标").split(",")[0]);
                map.put("y",rs.getString("经纬度坐标").split(",")[1]);
                list.add(map);
            }
            Set<String> pwkId = new HashSet<String>();
            for(int i = 0 ; i < list.size() ; i++){
                Map map = list.get(i);
                pwkId.add((String) map.get("pollutionCode"));
            }
            Iterator pwkIds = pwkId.iterator();
            while (pwkIds.hasNext()){
                Map<Object, Object> resultMap = new LinkedHashMap<Object, Object>();
                String currentPwkId = (String) pwkIds.next();
                double sumCod = 0;
                double sumNH = 0;
                String code = null;
                String name = null;
                String pollutionCode = null;
                String pollutionName = null;
                String x = null;
                String y = null;
                for(int i = 0 ; i < list.size() ; i++){
                    Map map = list.get(i);
                    if(currentPwkId.equals(map.get("pollutionCode"))){
                        sumCod = sumCod + (double)map.get("cod");
                        sumNH = sumNH + (double)map.get("inputValue");
                        if(code == null){
                            code = (String) map.get("code");
                        }
                        if(name == null){
                            name = (String) map.get("name");
                        }
                        if(pollutionCode == null){
                            pollutionCode = (String) map.get("pollutionCode");
                        }
                        if(pollutionName == null){
                            pollutionName = (String) map.get("pollutionName");
                        }
                        if(x == null){
                            x = (String) map.get("x");
                        }
                        if(y == null){
                            y = (String) map.get("y");
                        }
                    }
                }
                resultMap.put("code",code);
                resultMap.put("name",name);
                resultMap.put("pollutionCode",pollutionCode);
                resultMap.put("pollutionName",pollutionName);
                resultMap.put("cod",sumCod);
                resultMap.put("inputValue",sumNH);
                resultMap.put("x",x);
                resultMap.put("y",y);
                resultList.add(resultMap);
            }
            String sql1 = "select t1.水功能区_id,\n" +
                    "       t1.水功能区名称,\n" +
                    "       t2.排污口_id,\n" +
                    "       t2.入河排污口名称,\n" +
                    "       t2.经纬度坐标\n" +
                    "  from v_水功能区信息 t1, V_排污口信息 t2\n" +
                    " where t1.水功能区代码 = t2.水功能区代码\n" +
                    "   and t1.水功能区_id = ?\n";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,waterFounctionID);
            rs = ps.executeQuery();
            List<Map<Object,Object>> allList = new ArrayList<Map<Object, Object>>();
            Set<String> allPWKIds = new HashSet<String>();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                allPWKIds.add(rs.getString(3));
                map.put("code",rs.getString(1));
                map.put("name", rs.getString(2));
                map.put("pollutionCode",rs.getString(3));
                map.put("pollutionName",rs.getString(4));
                map.put("cod",0);
                map.put("inputValue",0);
                if(StringUtils.isNotEmpty(rs.getString(5))){
                    map.put("x",rs.getString(5).split(",")[0]);
                    map.put("y",rs.getString(5).split(",")[1]);
                }else {
                    map.put("x","");
                    map.put("y","");
                }
                allList.add(map);
            }
            Set<String> remainPWKIds = new HashSet<String>();
            remainPWKIds.addAll(allPWKIds);
            remainPWKIds.removeAll(pwkId);
            if(remainPWKIds.size()>0){
                Iterator remainPWKId = remainPWKIds.iterator();
                while (remainPWKId.hasNext()){
                    String currentPWKId = (String) remainPWKId.next();
                    for(int i = 0 ; i < allList.size() ; i++){
                        Map map = allList.get(i);
                        if(currentPWKId.equals(map.get("pollutionCode"))){
                            resultList.add(map);
                        }
                    }
                }
            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object,Object>> getXZQBubbleDetail(String water_body_id, String year, String xzq_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String xzqName = null;
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object,Object>>();

        try {
            List list = getAllXzqWaterQuality(water_body_id,year);
            connection = basicDataSource.getConnection();
            String sql = "select 行政区划名称 from v_行政区划 where 行政区_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,xzq_id);
            rs = ps.executeQuery();
            if(rs.next()){
                xzqName = rs.getString(1);
            }
            for(int i = 0 ; i < list.size() ; i++){
                List list1 = (List) list.get(i);
                for(int j = 0 ; j < list1.size() ; j++){
                    Map m = (Map) list1.get(j);
                    if(xzqName.equals(m.get("行政区划名称"))){
                        Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                        map.put("name","化学需氧量(COD)(t/a)");
                        map.put("value",m.get("化学需氧量(COD)"));
                        map.put("standard",m.get("化学需氧量(COD)限排"));
                        resultList.add(map);
                    }
                    if(xzqName.equals(m.get("行政区划名称"))){
                        Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                        map.put("name","氨氮(NH3-N)(t/a)");
                        map.put("value",m.get("氨氮(NH3-N)"));
                        map.put("standard",m.get("氨氮(NH3-N)限排"));
                        resultList.add(map);
                    }
                    if(xzqName.equals(m.get("行政区划名称"))){
                        Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                        map.put("name","长度达标率");
                        map.put("value",m.get("长度达标率"));
                        map.put("standard",m.get("水功能区长度最严格达标率"));
                        resultList.add(map);
                    }
                    if(xzqName.equals(m.get("行政区划名称"))){
                        Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                        map.put("name","个数达标率");
                        map.put("value",m.get("个数达标率"));
                        map.put("standard",m.get("水功能区数量最严格达标率"));
                        resultList.add(map);
                    }
                }
            }
            if(resultList.size()==0){
                Map<Object,Object> map1 = new LinkedHashMap<Object,Object>();
                map1.put("name","化学需氧量(COD)(t/a)");
                map1.put("value","");
                map1.put("standard","");

                Map<Object,Object> map2 = new LinkedHashMap<Object,Object>();
                map2.put("name","氨氮(NH3-N)(t/a)");
                map2.put("value","");
                map2.put("standard","");

                Map<Object,Object> map3 = new LinkedHashMap<Object,Object>();
                map3.put("name","长度达标率");
                map3.put("value","");
                map3.put("standard","");

                Map<Object,Object> map4 = new LinkedHashMap<Object,Object>();
                map4.put("name","个数达标率");
                map4.put("value","");
                map4.put("standard","");

                resultList.add(map1);
                resultList.add(map2);
                resultList.add(map3);
                resultList.add(map4);
            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<String, String>> getWaterFounctionWaterregimen(String water_body_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "SELECT S.水功能区_ID,\n" +
                    "       V.SUPER_OBJ_ID,\n" +
                    "       S.水功能区名称,\n" +
                    "       S.水质目标名称,\n" +
                    "       D.COLOR_VALUE AS 目标水质颜色\n" +
                    "  FROM ZDWARNER.V_水功能区信息            S,\n" +
                    "       ZDWARNER.V_水资源对象              V,\n" +
                    "       ZDWARNER.szpj_sys_断面水质类别定义 D\n" +
                    " WHERE S.水功能区_ID = V.OBJ_ID\n" +
                    "   AND V.map_id = ?\n" +
                    "   AND S.水质目标ID = D.LEVEL_ID\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,"928FE3EF219048C5BFEF749785476164".equals(water_body_id)?"taizihe":"yuqiao");
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> map = new LinkedHashMap<String,String>();
                map.put("id",rs.getString("水功能区_ID"));
                map.put("super_obj_id",rs.getString("SUPER_OBJ_ID"));
                map.put("name",rs.getString("水功能区名称"));
                map.put("目标水质名称",rs.getString("水质目标名称"));
                map.put("目标水质颜色",rs.getString("目标水质颜色"));
                list.add(map);
            }
        } catch (SQLException e) {
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
        return list;
    }

    @Override
    public void deleteModelData(String plan_id) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql ="delete from sdlm_output_计算结果表 where step_id in (select step_id from sdlm_biz_方案步骤定义 where plan_id = ?)";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,plan_id);
            ps.execute();
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public List<Map<Object, Object>> getXZQResultChart(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域的行政区划
        List<Map<String,String>> xzqIDAndNames = new ArrayList<Map<String, String>>();
        //存放太子河流域的行政区划（含有经纬度坐标）
        List<Map<String,String>> xzqlist = new ArrayList<Map<String, String>>();
        List<Map<Object,Object>> dataList = new ArrayList<Map<Object,Object>>();
        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3    \n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            ps.setString(2,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> map = new HashMap<String,String>();
                map.put("行政区ID",rs.getString(1));
                map.put("行政区划名称",rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID", rs.getString(4));
                xzqlist.add(map);
            }

            Set<String> xzqIDs = new HashSet<String>();
            for(int i = 0 ; i < xzqlist.size() ; i++){
                xzqIDs.add(xzqlist.get(i).get("行政区ID"));
            }
            Iterator xzqID = xzqIDs.iterator();

            while (xzqID.hasNext()){
                String currentXzqID = (String) xzqID.next();
                for(int j = 0 ; j < xzqlist.size() ; j++){
                    if(xzqlist.get(j).get("行政区ID").equals(currentXzqID)){
                        xzqIDAndNames.add(xzqlist.get(j));
                        break;
                    }
                }
            }

            for(int i = 0 ; i < xzqIDAndNames.size() ; i++){
                String xzqid = xzqIDAndNames.get(i).get("行政区ID");
                String xzqName = xzqIDAndNames.get(i).get("行政区划名称");
                String label = xzqIDAndNames.get(i).get("经纬度坐标");
                String superObjId = xzqIDAndNames.get(i).get("SUPER_OBJ_ID");
                int chaozai = 0;
                int buchaozai = 0;
                int linjie = 0;
                for(int j = 0 ; j < years.size() ; j++){
                    List list = getPollution(water_body_id,xzqid,years.get(j)+"");
                    if(list.size()>0){
                        for(int k = 0 ; k < list.size() ; k++){
                            Map map = (Map) list.get(k);
                            if("不超载".equals(map.get("水质达标状况"))){
                                buchaozai++;
                            }
                            if("临界超载".equals(map.get("水质达标状况"))){
                                linjie++;
                            }
                            if("超载区".equals(map.get("水质达标状况"))){
                                chaozai++;
                            }
                        }
                    }
                }
                DecimalFormat df  = new DecimalFormat("######0.00");
                double totalCount = buchaozai+linjie+chaozai;
                if(totalCount>0){
                    Map<Object,Object> dataMap = new LinkedHashMap<Object,Object>();
                    dataMap.put("行政区ID",xzqid);
                    dataMap.put("行政区划名称",xzqName);
                    dataMap.put("经纬度坐标",label);
                    dataMap.put("SUPER_OBJ_ID",superObjId);
                    dataMap.put("超载次数",chaozai);
                    dataMap.put("超载百分比",df.format(chaozai/totalCount*100)+"%");
                    dataMap.put("不超载次数",buchaozai);
                    dataMap.put("不超载百分比",df.format(buchaozai/totalCount*100)+"%");
                    dataMap.put("临界超载次数",linjie);
                    dataMap.put("临界超载百分比",df.format(linjie/totalCount*100)+"%");
                    dataList.add(dataMap);
                }
            }
        } catch (SQLException e) {
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
        return dataList;
    }

    @Override
    public List<Map<Object, Object>> getXZQPollutionQuantity(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域的行政区划
        List<Map<String,String>> xzqIDAndNames = new ArrayList<Map<String, String>>();
        //存放有经纬度坐标，super_obj_id的行政区划
        List<Map<String,String>> xzqlist = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();

        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3    \n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            ps.setString(2,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> map = new HashMap<String,String>();
                map.put("行政区ID",rs.getString(1));
                map.put("行政区划名称",rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID",rs.getString(4));
                xzqlist.add(map);
            }
            rs.close();
            ps.close();
            Set<String> xzqIDs = new HashSet<String>();
            for(int i = 0 ; i < xzqlist.size() ; i++){
                xzqIDs.add(xzqlist.get(i).get("行政区ID"));
            }
            Iterator xzqID = xzqIDs.iterator();

            while (xzqID.hasNext()){
                String currentXzqID = (String) xzqID.next();
                for(int j = 0 ; j < xzqlist.size() ; j++){
                    if(xzqlist.get(j).get("行政区ID").equals(currentXzqID)){
                        xzqIDAndNames.add(xzqlist.get(j));
                        break;
                    }
                }
            }

            for(int i = 0 ; i < xzqIDAndNames.size() ; i++){
                //存放每个行政区各个年份的信息
                List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
                String xzqid = xzqIDAndNames.get(i).get("行政区ID");
                String xzqName = xzqIDAndNames.get(i).get("行政区划名称");
                String label = xzqIDAndNames.get(i).get("经纬度坐标");
                String superObjId = xzqIDAndNames.get(i).get("SUPER_OBJ_ID");
                for(int j = 0 ; j < years.size() ; j++){
                    //存放某个行政区某一年的相关信息
                    List<Map<Object,Object>> dataList = new ArrayList<Map<Object,Object>>();
                    String sql1 = "select t4.上游行政区_id,\n" +
                            "       t5.行政区划名称,\n" +
                            "       t1.监测日期,\n" +
                            "       t4.水功能区_id,\n" +
                            "       t9.wfz_nm,\n" +
                            "       t6.item_id,\n" +
                            "       t6.sys_name,\n" +
                            "       t1.污染物入河量,\n" +
                            "       t7.水体_ID,\n" +
                            "       t8.水体名称\n" +
                            "  from szpj_biz_排污口污染物入河量 t1,\n" +
                            "       mgmb_入河排污口信息简表     t2,\n" +
                            "       wr_pdo_b                    t3,\n" +
                            "       mgmb_水功能区信息简表       t4,\n" +
                            "       v_行政区划                  t5,\n" +
                            "       szpj_sys_指标项             t6,\n" +
                            "       mgmb_rel_水功能区与水体     t7,\n" +
                            "       v_水体                      t8,\n" +
                            "       wr_wfz_b                    t9\n" +
                            " where t1.排污口_id = t2.入河排污口_id\n" +
                            "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                            "   and t2.wr_sd_code = t3.pdo_cd\n" +
                            "   and t3.wfz_cd = t4.wr_sd_code\n" +
                            "   and t4.上游行政区_id = t5.行政区_id\n" +
                            "   and t4.上游行政区_id = ?\n" +
                            "   and t6.item_id = t1.污染物指标_id\n" +
                            "   and t7.水功能区_id = t4.水功能区_id\n" +
                            "   and t7.水体_id = ?\n" +
                            "   and t7.水体_id = t8.水体_id\n" +
                            "   and t4.wr_sd_code = t9.wfz_cd\n" +
                            "union\n" +
                            "select t4.上游行政区_id,\n" +
                            "       t5.行政区划名称,\n" +
                            "       t1.监测日期,\n" +
                            "       t4.水功能区_id,\n" +
                            "       t9.wfz_nm,\n" +
                            "       t6.item_id,\n" +
                            "       t6.sys_name,\n" +
                            "       t1.污染物入河量,\n" +
                            "       t7.水体_ID,\n" +
                            "       t8.水体名称\n" +
                            "  from szpj_biz_排污口污染物入河量 t1,\n" +
                            "       mgmb_入河排污口信息简表     t2,\n" +
                            "       wr_pdo_b                    t3,\n" +
                            "       mgmb_水功能区信息简表       t4,\n" +
                            "       v_行政区划                  t5,\n" +
                            "       szpj_sys_指标项             t6,\n" +
                            "       mgmb_rel_水功能区与水体     t7,\n" +
                            "       v_水体                      t8,\n" +
                            "       wr_wfz_b                    t9\n" +
                            " where t1.排污口_id = t2.入河排污口_id\n" +
                            "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                            "   and t2.wr_sd_code = t3.pdo_cd\n" +
                            "   and t3.wfz_cd = t4.wr_sd_code\n" +
                            "   and t4.上游行政区_id = t5.行政区_id\n" +
                            "   and t4.上游行政区_id = ?\n" +
                            "   and t6.item_id = t1.污染物指标_id\n" +
                            "   and t7.水功能区_id = t4.水功能区_id\n" +
                            "   and t7.水体_id in\n" +
                            "       (select 支流_ID\n" +
                            "          from zdst_rel_干支流关系表\n" +
                            "         where 干流_id = ?)\n" +
                            "   and t7.水体_id = t8.水体_id\n" +
                            "   and t4.wr_sd_code = t9.wfz_cd";
                    ps = connection.prepareStatement(sql1);
                    ps.setString(1,years.get(j)+"");
                    ps.setString(2,years.get(j)+"");
                    ps.setString(3,xzqid);
                    ps.setString(4,water_body_id);
                    ps.setString(5,years.get(j)+"");
                    ps.setString(6,years.get(j)+"");
                    ps.setString(7,xzqid);
                    ps.setString(8,water_body_id);
                    rs = ps.executeQuery();
                    ResultSetMetaData metaData = rs.getMetaData();
                    while (rs.next()){
                        Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                        for(int k = 0 ; k < metaData.getColumnCount() ; k++){
                            String columnName = metaData.getColumnName(k+1);
                            map.put(columnName,rs.getObject(columnName));
                        }
                        dataList.add(map);
                    }
                    double COD = 0;
                    double NH = 0;
                    DecimalFormat df  = new DecimalFormat("######0.##");
                    for(int m = 0 ; m < dataList.size() ; m++){
                        Map map = dataList.get(m);
                        if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map.get("污染物入河量");
                            COD = COD + value.doubleValue();
                        }
                        if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map.get("污染物入河量");
                            NH = NH + value.doubleValue();
                        }
                    }
                    Map<Object,Object> dataMap = new LinkedHashMap<Object,Object>();
                    dataMap.put("year",years.get(j)+"");
                    dataMap.put("化学需氧量",COD==0?"":df.format(COD));
                    dataMap.put("氨氮",NH==0?"":df.format(NH));
                    list.add(dataMap);
                }
                Map<Object,Object> resultMap = new LinkedHashMap<Object,Object>();
                resultMap.put("行政区ID",xzqid);
                resultMap.put("行政区划名称",xzqName);
                resultMap.put("经纬度坐标",label);
                resultMap.put("SUPER_OBJ_ID",superObjId);
                resultMap.put("行政区年间入河量",list);
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getRiverPollutionQuantity(String water_body_id, String xzqid, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放当前行政区的河段
        Set<Map<Object,Object>> riverIDAndNames = new HashSet<Map<Object, Object>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();
        //存放当前行政区的数据
        List<Map<Object,Object>> dataList = new ArrayList<Map<Object,Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称,\n" +
                    "       t10.centerxy,\n" +
                    "       t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称,\n" +
                    "       t10.centerxy,\n" +
                    "       t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,xzqid);
            ps.setString(2,water_body_id);
            ps.setString(3,xzqid);
            ps.setString(4,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                for(int k = 0 ; k < metaData.getColumnCount() ; k++){
                    String columnName = metaData.getColumnName(k+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                dataList.add(map);
            }
            for(int i = 0 ; i < dataList.size() ; i++){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                map.put("水体ID",dataList.get(i).get("水体_ID"));
                map.put("水体名称",dataList.get(i).get("水体名称"));
                map.put("经纬度坐标",dataList.get(i).get("CENTERXY"));
                map.put("SUPER_OBJ_ID",dataList.get(i).get("SUPER_OBJ_ID"));
                riverIDAndNames.add(map);
            }
            Iterator riverIDAndName = riverIDAndNames.iterator();
            while (riverIDAndName.hasNext()){
                Map<Object,Object> currentRiverIDAndName = (Map)riverIDAndName.next();
                String currentRiverID = (String) currentRiverIDAndName.get("水体ID");
                String currentRiverName = (String) currentRiverIDAndName.get("水体名称");
                String currentRiverxy = (String) currentRiverIDAndName.get("经纬度坐标");
                String currentRiverSuperObjId = (String) currentRiverIDAndName.get("SUPER_OBJ_ID");
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
                //存放当前河段所有年份的信息
                List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
                //存放当前河段的信息
                List<Map<Object,Object>> riverList = new ArrayList<Map<Object, Object>>();
                for(int i = 0 ; i < dataList.size() ; i++){
                    if(dataList.get(i).get("水体_ID").equals(currentRiverID)){
                        riverList.add(dataList.get(i));
                    }
                }
                for(int i = 0; i < years.size() ; i++){
                    //存放当前河段当前年份的信息
                    List<Map<Object,Object>> yearList = new ArrayList<Map<Object, Object>>();
                    double COD = 0;
                    double NH = 0;
                    String currentYear = ""+years.get(i);
                    for(int j = 0 ; j < riverList.size() ; j++){
                        if(sdf.format(riverList.get(j).get("监测日期")).equals(currentYear)){
                            yearList.add(riverList.get(j));
                        }
                    }

                    for(int j = 0 ; j < yearList.size() ; j++){
                        Map map = yearList.get(j);
                        if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map.get("污染物入河量");
                            COD = COD + value.doubleValue();
                        }
                        if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map.get("污染物入河量");
                            NH = NH + value.doubleValue();
                        }
                    }
                    Map map = new LinkedHashMap();
                    map.put("year",currentYear);
                    map.put("COD",COD==0?"":COD);
                    map.put("NH",NH==0?"":NH);
                    list.add(map);
                }
                Map<Object,Object> resultMap = new LinkedHashMap<Object,Object>();
                resultMap.put("水体ID",currentRiverID);
                resultMap.put("水体名称",currentRiverName);
                resultMap.put("经纬度坐标",currentRiverxy);
                resultMap.put("SUPER_OBJ_ID",currentRiverSuperObjId);
                resultMap.put("河段年间入河量",list);
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
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
        return resultList;

    }

    @Override
    public List<Map<Object, Object>> getWaterFounctionPollutionQuantity(String water_body_id, String xzqid, String riverID, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放当前行政区的河段
        Set<Map<Object,Object>> riverIDAndNames = new HashSet<Map<Object, Object>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();
        //存放当前行政区的数据
        List<Map<Object,Object>> dataList = new ArrayList<Map<Object,Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称,\n" +
                    "       t10.centerxy,\n" +
                    "       t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t4.水功能区_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称,\n" +
                    "       t10.centerxy,\n" +
                    "       t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t4.水功能区_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,xzqid);
            ps.setString(2,water_body_id);
            ps.setString(3,xzqid);
            ps.setString(4,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<Object,Object> map = new LinkedHashMap<Object,Object>();
                for(int k = 0 ; k < metaData.getColumnCount() ; k++){
                    String columnName = metaData.getColumnName(k+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                dataList.add(map);
            }
            //存放当前河段的信息
            List<Map<Object,Object>> riverList = new ArrayList<Map<Object, Object>>();
            for(int i = 0 ; i < dataList.size() ; i++){
                if(dataList.get(i).get("水体_ID").equals(riverID)){
                    riverList.add(dataList.get(i));
                }
            }
            //存放当前河段的水功能区
            Set<Map<Object,Object>> waterFounctionIDAndNames = new HashSet<Map<Object, Object>>();
            for(int i = 0 ; i < riverList.size() ; i++){
                Map<Object,Object> map = new HashMap<Object,Object>();
                map.put("水功能区ID",riverList.get(i).get("水功能区_ID"));
                map.put("水功能区名称",riverList.get(i).get("WFZ_NM"));
                map.put("经纬度坐标",riverList.get(i).get("CENTERXY"));
                map.put("SUPER_OBJ_ID",riverList.get(i).get("SUPER_OBJ_ID"));
                waterFounctionIDAndNames.add(map);
            }
            Iterator waterFounctionIDAndName = waterFounctionIDAndNames.iterator();
            while (waterFounctionIDAndName.hasNext()){
                Map<Object,Object> map = (Map<Object, Object>) waterFounctionIDAndName.next();
                String currentWaterFounctionID = (String) map.get("水功能区ID");
                String currentWaterFounctionName = (String) map.get("水功能区名称");
                String currentWaterFounctionXY = (String) map.get("经纬度坐标");
                String currentWaterFounctionSuperObjId = (String) map.get("SUPER_OBJ_ID");
                SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
                //存放当前水功能区所有年份的信息
                List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
                //存放当前河段的水功能区信息
                List<Map<Object,Object>> waterFounctionList = new ArrayList<Map<Object, Object>>();
                for(int j = 0 ; j < riverList.size() ; j++){
                    if(riverList.get(j).get("水功能区_ID").equals(currentWaterFounctionID)){
                        waterFounctionList.add(riverList.get(j));
                    }
                }

                for(int j = 0 ; j < years.size() ; j++){
                    String currentYear = years.get(j)+"";
                    //存放当前河段的水功能区的某个年份的信息
                    List<Map<Object,Object>> yearList = new ArrayList<Map<Object, Object>>();
                    double COD = 0;
                    double NH = 0;
                    for(int k = 0 ; k < waterFounctionList.size() ; k++){
                        if(sdf.format(waterFounctionList.get(k).get("监测日期")).equals(currentYear)){
                            yearList.add(waterFounctionList.get(k));
                        }
                    }
                    for(int k = 0 ; k < yearList.size() ; k++){
                        Map map1 = yearList.get(k);
                        if("化学需氧量(COD)".equals(map1.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map1.get("污染物入河量");
                            COD = COD + value.doubleValue();
                        }
                        if("氨氮(NH3-N)".equals(map1.get("SYS_NAME"))){
                            BigDecimal value = (BigDecimal)map1.get("污染物入河量");
                            NH = NH + value.doubleValue();
                        }
                    }
                    Map map1 = new LinkedHashMap();
                    map1.put("year",currentYear);
                    map1.put("COD",COD==0?"":COD);
                    map1.put("NH",NH==0?"":NH);
                    list.add(map1);
                }
                Map resultMap = new LinkedHashMap();
                resultMap.put("水功能区ID",currentWaterFounctionID);
                resultMap.put("水功能区名称",currentWaterFounctionName);
                resultMap.put("经纬度坐标",currentWaterFounctionXY);
                resultMap.put("SUPER_OBJ_ID",currentWaterFounctionSuperObjId);
                resultMap.put("水功能区年间入河量",list);
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getXZQPollutionExcessive(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域的行政区划
        List<Map<String,String>> xzqIDAndNames = new ArrayList<Map<String, String>>();
        //存放有经纬度坐标，super_obj_id的行政区划
        List<Map<String,String>> xzqlist = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();

        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3    \n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, water_body_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("行政区ID", rs.getString(1));
                map.put("行政区划名称", rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID", rs.getString(4));
                xzqlist.add(map);
            }
            rs.close();
            ps.close();

            Set<String> xzqIDs = new HashSet<String>();
            for(int i = 0 ; i < xzqlist.size() ; i++){
                xzqIDs.add(xzqlist.get(i).get("行政区ID"));
            }
            Iterator xzqID = xzqIDs.iterator();

            while (xzqID.hasNext()){
                String currentXzqID = (String) xzqID.next();
                for(int j = 0 ; j < xzqlist.size() ; j++){
                    if(xzqlist.get(j).get("行政区ID").equals(currentXzqID)){
                        xzqIDAndNames.add(xzqlist.get(j));
                        break;
                    }
                }
            }

            for (int i = 0; i < xzqIDAndNames.size(); i++) {
                //存放每个行政区各个年份的信息
                List<Map<Object, Object>> list = new ArrayList<Map<Object, Object>>();
                String xzqid = xzqIDAndNames.get(i).get("行政区ID");
                String xzqName = xzqIDAndNames.get(i).get("行政区划名称");
                String label = xzqIDAndNames.get(i).get("经纬度坐标");
                String superObjId = xzqIDAndNames.get(i).get("SUPER_OBJ_ID");
                List list1 = new ArrayList();
                int chaobiao = 0;
                int buchaobiao = 0;
                for (int j = 0; j < years.size(); j++) {
                    String currentYear = years.get(j)+"";
                    Map map = getPollutionExcessive(water_body_id,xzqid,currentYear);
                    if(map.get("水质达标状况").equals("不超标")){
                        buchaobiao++;
                    }else if(map.get("水质达标状况").equals("超标")){
                        chaobiao++;
                    }
                }
                double totalCount = chaobiao + buchaobiao;
                DecimalFormat df  = new DecimalFormat("######0.00");
                if(totalCount>0){
                    Map resultMap = new LinkedHashMap();
                    resultMap.put("行政区_ID",xzqid);
                    resultMap.put("行政区划名称",xzqName);
                    resultMap.put("经纬度坐标",label);
                    resultMap.put("SUPER_OBJ_ID",superObjId);
                    resultMap.put("超标次数",chaobiao);
                    resultMap.put("超标百分比",df.format(chaobiao/totalCount*100)+"%");
                    resultMap.put("不超标次数",buchaobiao);
                    resultMap.put("不超标百分比",df.format(buchaobiao/totalCount*100)+"%");
                    resultList.add(resultMap);
                }
            }
        }catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getRiverPollutionExcessive(String water_body_id, String xzqid, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();
        try{
            connection = basicDataSource.getConnection();
            String sql = "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,xzqid);
            ps.setString(2,water_body_id);
            ps.setString(3,xzqid);
            ps.setString(4,water_body_id);
            rs = ps.executeQuery();
            List riverList = new ArrayList();
            while (rs.next()){
                Map map = new HashMap();
                map.put("水体_ID",rs.getString(1));
                map.put("水体名称",rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID",rs.getString(4));
                riverList.add(map);
            }
            //存放每个行政区各个年份的信息
            List<Map<Object, Object>> list = new ArrayList<Map<Object, Object>>();
            List list1 = new ArrayList();

            for(int k = 0 ; k < riverList.size() ; k++){
                int chaobiao = 0;
                int buchaobiao = 0;
                Map map = (Map) riverList.get(k);
                String riverID = (String) map.get("水体_ID");
                String riverName = (String) map.get("水体名称");
                String riverXY = (String) map.get("经纬度坐标");
                String riverSuperObjId = (String) map.get("SUPER_OBJ_ID");
                for (int j = 0; j < years.size(); j++) {
                    String currentYear = years.get(j)+"";
                    Map map1 = getPollutionExcessive(water_body_id,xzqid,riverID,currentYear);
                    if(map1.get("水质达标状况").equals("不超标")){
                        buchaobiao++;
                    }else if(map1.get("水质达标状况").equals("超标")){
                        chaobiao++;
                    }
                }
                double totalCount = chaobiao + buchaobiao;
                DecimalFormat df  = new DecimalFormat("######0.00");
                if(totalCount>0){
                    Map resultMap = new LinkedHashMap();
                    resultMap.put("水体_ID",riverID);
                    resultMap.put("水体名称",riverName);
                    resultMap.put("经纬度坐标",riverXY);
                    resultMap.put("SUPER_OBJ_ID",riverSuperObjId);
                    resultMap.put("超标次数",chaobiao);
                    resultMap.put("超标百分比",df.format(chaobiao/totalCount*100)+"%");
                    resultMap.put("不超标次数",buchaobiao);
                    resultMap.put("不超标百分比",df.format(buchaobiao/totalCount*100)+"%");
                    resultList.add(resultMap);
                }
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getXZQWaterRate(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域内行政区信息
        List<Map<String,String>> xzqIDAndNames = new ArrayList<Map<String, String>>();
        //
        List<Map<String,String>> xzqlist = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();

        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3    \n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, water_body_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("行政区ID", rs.getString(1));
                map.put("行政区划名称", rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID",rs.getString(4));
                xzqlist.add(map);
            }
            Set<String> xzqIDs = new HashSet<String>();
            for(int i = 0 ; i < xzqlist.size() ; i++){
                xzqIDs.add(xzqlist.get(i).get("行政区ID"));
            }
            Iterator xzqID = xzqIDs.iterator();

            while (xzqID.hasNext()){
                String currentXzqID = (String) xzqID.next();
                for(int j = 0 ; j < xzqlist.size() ; j++){
                    if(xzqlist.get(j).get("行政区ID").equals(currentXzqID)){
                        xzqIDAndNames.add(xzqlist.get(j));
                        break;
                    }
                }
            }
            for(int i = 0 ; i < xzqIDAndNames.size() ; i++){
                //存放每个行政区各个年份的信息
                List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
                String xzqid = xzqIDAndNames.get(i).get("行政区ID");
                String xzqName = xzqIDAndNames.get(i).get("行政区划名称");
                String label = xzqIDAndNames.get(i).get("经纬度坐标");
                String superObjId = xzqIDAndNames.get(i).get("SUPER_OBJ_ID");
                for(int j = 0 ; j < years.size() ; j++){
                    List<Map<Object,Object>> xzqList = getxzqList(water_body_id, xzqid);
                    Map map1 = getSzyWaterQuality(xzqList, water_body_id, xzqid, years.get(j) + "");
                    if(map1!=null && map1.size()>0){
                        map1.put("year", years.get(j) + "");
                        map1.remove("totalCount");
                        map1.remove("attainedCount");
                        map1.remove("attainedPercent");
                        map1.remove("totalLength");
                        map1.remove("attainedLength");
                        map1.remove("lengthPercent");
                        map1.remove("x");
                        map1.remove("y");
                        map1.remove("水功能区详细信息");
                        list.add(map1);
                    }else {
                        Map map2 = new LinkedHashMap();
                        map2.put("个数达标率","");
                        map2.put("长度达标率","");
                        map2.put("year", years.get(j) + "");
                        list.add(map2);
                    }
                }
                Map<Object,Object> resultMap = new LinkedHashMap<Object,Object>();
                resultMap.put("行政区ID",xzqid);
                resultMap.put("行政区划名称",xzqName);
                resultMap.put("经纬度坐标",label);
                resultMap.put("SUPER_OBJ_ID",superObjId);
                resultMap.put("年间行政区水质达标率",list);
                resultList.add(resultMap);
            }
        }catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getRiverWaterRate(String water_body_id, String xzqid, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域内行政区信息
        List<Map<String,String>> riverIDAndNames = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();

        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, xzqid);
            ps.setString(2, water_body_id);
            ps.setString(3, xzqid);
            ps.setString(4, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("水体ID", rs.getString(1));
                map.put("水体名称", rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID",rs.getString(4));
                riverIDAndNames.add(map);
            }
            for(int i = 0 ; i < riverIDAndNames.size() ; i++){
                //存放每个行政区各个年份的信息
                List<Map<Object,Object>> list = new ArrayList<Map<Object, Object>>();
                String riverId = riverIDAndNames.get(i).get("水体ID");
                String riverName = riverIDAndNames.get(i).get("水体名称");
                String riverXY = riverIDAndNames.get(i).get("经纬度坐标");
                String riverSuperObjId = riverIDAndNames.get(i).get("SUPER_OBJ_ID");
                for(int j = 0 ; j < years.size() ; j++){
                    List<Map<Object,Object>> xzqList = getOnexzqList(riverId, xzqid);
                    Map map1 = getSzyWaterQuality(xzqList, water_body_id, xzqid, years.get(j) + "");
                    if(map1!=null && map1.size()>0){
                        map1.put("year", years.get(j) + "");
                        map1.remove("totalCount");
                        map1.remove("attainedCount");
                        map1.remove("attainedPercent");
                        map1.remove("totalLength");
                        map1.remove("attainedLength");
                        map1.remove("lengthPercent");
                        map1.remove("x");
                        map1.remove("y");
                        map1.remove("水功能区详细信息");
                        list.add(map1);
                    }else {
                        Map map2 = new LinkedHashMap();
                        map2.put("个数达标率","");
                        map2.put("长度达标率","");
                        map2.put("year", years.get(j) + "");
                        list.add(map2);
                    }
                }
                Map<Object,Object> resultMap = new LinkedHashMap<Object,Object>();
                resultMap.put("水体ID",riverId);
                resultMap.put("水体名称",riverName);
                resultMap.put("经纬度坐标",riverXY);
                resultMap.put("SUPER_OBJ_ID",riverSuperObjId);
                resultMap.put("年间河段水质达标率",list);
                resultList.add(resultMap);
            }
        }catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getXZQWaterRateExcessive(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域内行政区信息
        List<Map<String,String>> xzqIDAndNames = new ArrayList<Map<String, String>>();
        //存放太子河流域内行政区信息（经纬度坐标）
        List<Map<String,String>> xzqlist = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3\n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id = ?\n" +
                    "   and t2.上游行政区_id is not null\n" +
                    "union\n" +
                    "select distinct t2.上游行政区_id,\n" +
                    "                t3.行政区划名称,\n" +
                    "                t2.经纬度坐标,\n" +
                    "                t3.wr_sd_code\n" +
                    "  from mgmb_rel_水功能区与水体 t1, mgmb_水功能区信息简表 t2, v_行政区划 t3    \n" +
                    " where t1.水功能区_id = t2.水功能区_id\n" +
                    "   and t2.上游行政区_id = t3.行政区_id\n" +
                    "   and t1.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_ID = ?)\n" +
                    "   and t2.上游行政区_id is not null\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, water_body_id);
            ps.setString(2, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("行政区ID", rs.getString(1));
                map.put("行政区划名称", rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID", rs.getString(4));
                xzqlist.add(map);
            }
            rs.close();
            ps.close();

            Set<String> xzqIDs = new HashSet<String>();
            for(int i = 0 ; i < xzqlist.size() ; i++){
                xzqIDs.add(xzqlist.get(i).get("行政区ID"));
            }
            Iterator xzqID = xzqIDs.iterator();

            while (xzqID.hasNext()){
                String currentXzqID = (String) xzqID.next();
                for(int j = 0 ; j < xzqlist.size() ; j++){
                    if(xzqlist.get(j).get("行政区ID").equals(currentXzqID)){
                        xzqIDAndNames.add(xzqlist.get(j));
                        break;
                    }
                }
            }

            for (int i = 0; i < xzqIDAndNames.size(); i++) {
                //存放每个行政区各个年份的信息
                List<Map<Object, Object>> list = new ArrayList<Map<Object, Object>>();
                String xzqid = xzqIDAndNames.get(i).get("行政区ID");
                String xzqName = xzqIDAndNames.get(i).get("行政区划名称");
                String label = xzqIDAndNames.get(i).get("经纬度坐标");
                String superObjId = xzqIDAndNames.get(i).get("SUPER_OBJ_ID");
                List list1 = new ArrayList();
                int chaobiao = 0;
                int buchaobiao = 0;
                for (int j = 0; j < years.size(); j++) {
                    String currentYear = years.get(j)+"";
                    Map dbMap = new HashMap();
                    String sql1 = "select distinct t1.年份,\n" +
                            "       t1.行政区划_ID,           \n" +
                            "       t1.水功能区数量最严格达标率,\n" +
                            "       t1.水功能区长度最严格达标率\n" +
                            "  from szpj_sys_污染物限排标准 t1, szpj_sys_指标项 t2\n" +
                            " where t1.污染物指标_id = t2.item_id\n" +
                            "   and 年份 = ? and t1.行政区划_ID = ?";
                    ps = connection.prepareStatement(sql1);
                    ps.setInt(1, (Integer) years.get(j));
                    ps.setString(2,xzqid);
                    rs = ps.executeQuery();
                    while (rs.next()){
                        dbMap.put("水功能区数量最严格达标率",rs.getObject(3));
                        dbMap.put("水功能区长度最严格达标率", rs.getObject(4));
                    }

                    List<Map<Object,Object>> xzqList = getxzqList(water_body_id, xzqid);
                    Map map = getSzyWaterQuality(xzqList, water_body_id, xzqid, currentYear);
                    if (map!=null && map.size()>0){
                        String countExcessive = (String)map.get("个数达标率");
                        String lengthExcessive = (String)map.get("长度达标率");
                        double countExcessiveToDouble = Double.parseDouble(countExcessive.substring(0, countExcessive.indexOf("%")));
                        double lengthExcessiveToDouble = Double.parseDouble(lengthExcessive.substring(0, lengthExcessive.indexOf("%")));
                        if (dbMap.size()>0){
                            BigDecimal countValue = (BigDecimal) dbMap.get("水功能区数量最严格达标率");
                            BigDecimal lengthValue = (BigDecimal) dbMap.get("水功能区长度最严格达标率");
                            if(countExcessiveToDouble<countValue.doubleValue()&&lengthExcessiveToDouble<lengthValue.doubleValue()){
                                chaobiao++;
                            }else if(countExcessiveToDouble>countValue.doubleValue()&&lengthExcessiveToDouble>lengthValue.doubleValue()){
                                buchaobiao++;
                            }
                        }
                    }
                }
                double totalCount = chaobiao + buchaobiao;
                DecimalFormat df  = new DecimalFormat("######0.00");
                if(totalCount>0){
                    Map resultMap = new LinkedHashMap();
                    resultMap.put("行政区_ID",xzqid);
                    resultMap.put("行政区划名称",xzqName);
                    resultMap.put("经纬度坐标",label);
                    resultMap.put("SUPER_OBJ_ID",superObjId);
                    resultMap.put("超标次数",chaobiao);
                    resultMap.put("超标百分比",df.format(chaobiao/totalCount*100)+"%");
                    resultMap.put("不超标次数",buchaobiao);
                    resultMap.put("不超标百分比",df.format(buchaobiao/totalCount*100)+"%");
                    resultList.add(resultMap);
                }
            }
        } catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<Object, Object>> getRiverWaterRateExcessive(String water_body_id, String xzqid, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int year = Integer.valueOf(begin_year)-1;
        //years存放年份
        List years = new ArrayList();
        while (year < Integer.valueOf(end_year)){
            year++;
            years.add(year);
        }
        //存放太子河流域内行政区信息
        List<Map<String,String>> riverIDAndNames = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object, Object>>();

        try {
            connection = basicDataSource.getConnection();
            //找到太子河流域内所有行政区
            String sql = "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t7.水体_ID, t8.水体名称, t10.centerxy, t10.super_obj_id\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9,\n" +
                    "       v_水资源对象                t10\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and t10.obj_id = t8.水体_id\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, xzqid);
            ps.setString(2, water_body_id);
            ps.setString(3, xzqid);
            ps.setString(4, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                Map<String, String> map = new HashMap<String, String>();
                map.put("水体ID", rs.getString(1));
                map.put("水体名称", rs.getString(2));
                map.put("经纬度坐标",rs.getString(3));
                map.put("SUPER_OBJ_ID",rs.getString(4));
                riverIDAndNames.add(map);
            }
            rs.close();
            ps.close();

            for (int i = 0; i < riverIDAndNames.size(); i++) {
                //存放每个行政区各个年份的信息
                List<Map<Object, Object>> list = new ArrayList<Map<Object, Object>>();
                String riverId = riverIDAndNames.get(i).get("水体ID");
                String riverName = riverIDAndNames.get(i).get("水体名称");
                String riverXY = riverIDAndNames.get(i).get("经纬度坐标");
                String riverSuperObjId = riverIDAndNames.get(i).get("SUPER_OBJ_ID");
                int chaobiao = 0;
                int buchaobiao = 0;
                for (int j = 0; j < years.size(); j++) {
                    Map dbMap = new HashMap();
                    String sql1 = "select distinct t1.年份,\n" +
                            "       t1.行政区划_ID,           \n" +
                            "       t1.水功能区数量最严格达标率,\n" +
                            "       t1.水功能区长度最严格达标率\n" +
                            "  from szpj_sys_污染物限排标准 t1, szpj_sys_指标项 t2\n" +
                            " where t1.污染物指标_id = t2.item_id\n" +
                            "   and 年份 = ? and t1.行政区划_ID = ?";
                    ps = connection.prepareStatement(sql1);
                    ps.setInt(1, (Integer) years.get(j));
                    ps.setString(2,xzqid);
                    rs = ps.executeQuery();
                    while (rs.next()){
                        dbMap.put("水功能区数量最严格达标率",rs.getObject(3));
                        dbMap.put("水功能区长度最严格达标率", rs.getObject(4));
                    }
                    List<Map<Object, Object>> xzqList = getOnexzqList(riverId, xzqid);
                    Map map1 = getSzyWaterQuality(xzqList, water_body_id, xzqid, years.get(j) + "");
                    if (map1!=null && map1.size()>0){
                        String countExcessive = (String)map1.get("个数达标率");
                        String lengthExcessive = (String)map1.get("长度达标率");
                        double countExcessiveToDouble = Double.parseDouble(countExcessive.substring(0, countExcessive.indexOf("%")));
                        double lengthExcessiveToDouble = Double.parseDouble(lengthExcessive.substring(0, lengthExcessive.indexOf("%")));
                        if (dbMap.size()>0){
                            BigDecimal countValue = (BigDecimal) dbMap.get("水功能区数量最严格达标率");
                            BigDecimal lengthValue = (BigDecimal) dbMap.get("水功能区长度最严格达标率");
                            if(countExcessiveToDouble<countValue.doubleValue()&&lengthExcessiveToDouble<lengthValue.doubleValue()){
                                chaobiao++;
                            }else if(countExcessiveToDouble>countValue.doubleValue()&&lengthExcessiveToDouble>lengthValue.doubleValue()){
                                buchaobiao++;
                            }
                        }
                    }
                }
                double totalCount = chaobiao + buchaobiao;
                DecimalFormat df  = new DecimalFormat("######0.00");
                if(totalCount>0){
                    Map resultMap = new LinkedHashMap();
                    resultMap.put("水体ID",riverId);
                    resultMap.put("水体名称",riverName);
                    resultMap.put("经纬度坐标",riverXY);
                    resultMap.put("SUPER_OBJ_ID",riverSuperObjId);
                    resultMap.put("超标次数",chaobiao);
                    resultMap.put("超标百分比",df.format(chaobiao/totalCount*100)+"%");
                    resultMap.put("不超标次数",buchaobiao);
                    resultMap.put("不超标百分比",df.format(buchaobiao/totalCount*100)+"%");
                    resultList.add(resultMap);
                }
            }
        }catch (SQLException e) {
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
        return resultList;
    }

    @Override
    public List<Map<String, Object>> getRiverSZPJCount(String water_body_id, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int count  = 0;
        //存放结果
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        //存放待整理数据
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
                call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
                call.setString(1,water_body_id);
                call.setString(2,"year");
                call.setString(3,"");
                call.setString(4, i + "-01-01");
                call.setString(5, i + "-12-31");
                call.registerOutParameter(6, OracleTypes.CURSOR);
                call.registerOutParameter(7, OracleTypes.CURSOR);
                call.registerOutParameter(8, OracleTypes.CURSOR);
                call.execute();
                rs = (ResultSet) call.getObject(6);
                while (rs.next()){
                    Map<String,String> map = new HashMap<String,String>();
                    map.put("riverID",rs.getString("评价对象_ID"));
                    map.put("riverName",rs.getString("WR_SD_NM"));
                    map.put("水质状况",rs.getString("水质状况"));
                    list.add(map);
                }
                connection.commit();
                rs.close();
                call.close();
            }

            Set<String> riverNames = new HashSet<String>();
            for(int i = 0 ; i < list.size() ; i++){
                String riverName = list.get(i).get("riverName");
                riverNames.add(riverName);
            }
            Iterator riverName = riverNames.iterator();
            List<Map<String,Object>> resultTempList = new ArrayList<Map<String, Object>>();
            while (riverName.hasNext()){
                String currentRiverName = (String) riverName.next();
                String currentRiverID = null;
                List<Map<String,String>> riverList = new ArrayList<Map<String, String>>();
                for(int j = 0 ; j < list.size() ; j++){
                    if(list.get(j).get("riverName").equals(currentRiverName)){
                        riverList.add(list.get(j));
                        if(currentRiverID == null){
                            currentRiverID = list.get(j).get("riverID");
                        }
                    }
                }

                int countStep1 = 0;//优
                int countStep2 = 0;//良好
                int countStep3 = 0;//轻度污染
                int countStep4 = 0;//中度污染
                int countStep5 = 0;//重度污染

                for(int j = 0 ; j < riverList.size() ; j++){
                    String riverCondition = riverList.get(j).get("水质状况");
                    if("优".equals(riverCondition)){
                        countStep1++;
                    }
                    if("良好".equals(riverCondition)){
                        countStep2++;
                    }
                    if("轻度污染".equals(riverCondition)){
                        countStep3++;
                    }
                    if("中度污染".equals(riverCondition)){
                        countStep4++;
                    }
                    if("重度污染".equals(riverCondition)){
                        countStep5++;
                    }
                }
                count = countStep1 + countStep2 + countStep3 + countStep4 + countStep5;
                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                resultMap.put("riverID",currentRiverID);
                resultMap.put("riverName",currentRiverName);
                resultMap.put("次数总计",count);
                resultMap.put("优",countStep1);
                resultMap.put("良好",countStep2);
                resultMap.put("轻度污染",countStep3);
                resultMap.put("中度污染",countStep4);
                resultMap.put("重度污染",countStep5);
                resultTempList.add(resultMap);
            }
            Map<String,Object> tempMap = new HashMap<String,Object>();
            for(int i = 0 ; i < resultTempList.size() ; i++){
                 Map<String,Object> map = resultTempList.get(i);
                if(map.get("riverName").equals("太子河")){
                    resultList.add(map);
                    tempMap.putAll(map);
                }
            }
            resultTempList.remove(tempMap);
            resultList.addAll(resultTempList);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(call!= null)
                    call.close();
                if(connection != null)
                connection.close();
            } catch (SQLException e) {
                connection.rollback();
                e.printStackTrace();
                throw e;
            }
        }

        return resultList;
    }

//    @Override
//    public List<Map<String, Object>> getWaterFounctionSZPJCount2(String water_body_id, String riverID, String begin_year, String end_year) throws SQLException {
//        Connection connection = null;
//        CallableStatement call = null;
//        ResultSet rs = null;
//        int count  = 0;
//        //存放结果
//        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
//        //存放待整理数据
//        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
//        try {
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
//                call.setString(1,water_body_id);
//                call.setString(2,"year");
//                call.setString(3,riverID);
//                call.setString(4, i + "-01-01");
//                call.setString(5, i + "-12-31");
//                call.registerOutParameter(6, OracleTypes.CURSOR);
//                call.registerOutParameter(7, OracleTypes.CURSOR);
//                call.registerOutParameter(8, OracleTypes.CURSOR);
//                call.execute();
//                rs = (ResultSet) call.getObject(7);
//                while (rs.next()){
//                    Map<String,String> map = new HashMap<String,String>();
//                    map.put("waterFounctionID",rs.getString("评价对象_ID"));
//                    map.put("waterFounctionName",rs.getString("WR_SD_NM"));
//                    map.put("水质状况",rs.getString("水质状况"));
//                    map.put("是否达标",rs.getString("是否达标"));
//                    list.add(map);
//                }
//                connection.commit();
//                rs.close();
//                rs = null;
//            }
//
//            Set<String> waterFounctionNames = new HashSet<String>();
//
//            for(int i = 0 ; i < list.size() ; i++){
//                String waterFounctionName = list.get(i).get("waterFounctionName");
//                waterFounctionNames.add(waterFounctionName);
//
//            }
//            Iterator waterFounctionName = waterFounctionNames.iterator();
//            while (waterFounctionName.hasNext()){
//                String currentWaterFounctionID = null;
//                String currentWaterFounctionName = (String) waterFounctionName.next();
//                List<Map<String,String>> waterFounctionList = new ArrayList<Map<String, String>>();
//                for(int j = 0 ; j < list.size() ; j++){
//                    String name =  list.get(j).get("waterFounctionName");
//                    if(name.equals(currentWaterFounctionName)){
//                        if(currentWaterFounctionID == null){
//                            currentWaterFounctionID = list.get(j).get("waterFounctionID");
//                        }
//                        waterFounctionList.add(list.get(j));
//                    }
//                }
//                int chaobiaoCount = 0;
//                int countStep1 = 0;//优
//                int countStep2 = 0;//良好
//                int countStep3 = 0;//轻度污染
//                int countStep4 = 0;//中度污染
//                int countStep5 = 0;//重度污染
//
//                for(int j = 0 ; j < waterFounctionList.size() ; j++){
//                    if("1".equals(waterFounctionList.get(j).get("是否达标"))){
//                        chaobiaoCount++;
//                    }
//                    String waterFounctionCondition = (String) waterFounctionList.get(j).get("水质状况");
//                    if("优".equals(waterFounctionCondition)){
//                        countStep1++;
//                    }
//                    if("良好".equals(waterFounctionCondition)){
//                        countStep2++;
//                    }
//                    if("轻度污染".equals(waterFounctionCondition)){
//                        countStep3++;
//                    }
//                    if("中度污染".equals(waterFounctionCondition)){
//                        countStep4++;
//                    }
//                    if("重度污染".equals(waterFounctionCondition)){
//                        countStep5++;
//                    }
//                }
//                count = countStep1 + countStep2 + countStep3 + countStep4 + countStep5;
//                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//                DecimalFormat df  = new DecimalFormat("######0.00");
//                resultMap.put("waterFounctionID",currentWaterFounctionID);
//                resultMap.put("waterFounctionName",currentWaterFounctionName);
//                resultMap.put("次数总计",count);
//                resultMap.put("优",countStep1);
//                resultMap.put("良好",countStep2);
//                resultMap.put("轻度污染",countStep3);
//                resultMap.put("中度污染",countStep4);
//                resultMap.put("重度污染",countStep5);
//                resultMap.put("超标次数",chaobiaoCount);
//                resultMap.put("超标百分比",df.format(chaobiaoCount/Double.parseDouble(count+"")*100)+"%");
//                resultList.add(resultMap);
//            }
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                if(rs != null)
//                    rs.close();
//                if(call!= null)
//                    call.close();
//                if(connection != null)
//                    connection.close();
//            } catch (SQLException e) {
//                connection.rollback();
//                e.printStackTrace();
//                throw e;
//            }
//        }
//        return resultList;
//    }

//    @Override
//    public Map<String, Object> getWaterFounctionSZPJCountDetail2(String water_body_id, String riverId,String waterFounctionID, String begin_year, String end_year) throws SQLException {
//        Connection connection = null;
//        CallableStatement call = null;
//        PreparedStatement ps = null;
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//        //存放结果
//        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//        //存放整理数据
//        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
//        List<Map<String,Object>> dmList = new ArrayList<Map<String, Object>>();
//        try {
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
//                call.setString(1,water_body_id);
//                call.setString(2,"year");
//                call.setString(3,riverId);
//                call.setString(4, i + "-01-01");
//                call.setString(5, i + "-12-31");
//                call.registerOutParameter(6, OracleTypes.CURSOR);
//                call.registerOutParameter(7, OracleTypes.CURSOR);
//                call.registerOutParameter(8, OracleTypes.CURSOR);
//                call.execute();
//                rs1 = (ResultSet) call.getObject(7);
//                rs2 = (ResultSet) call.getObject(8);
//                while (rs1.next()){
//                    Map<String,String> map = new HashMap<String,String>();
//                    map.put("waterFounctionID",rs1.getString("评价对象_ID"));
//                    map.put("waterFounctionName",rs1.getString("WR_SD_NM"));
//                    map.put("水质状况",rs1.getString("水质状况"));
//                    map.put("是否达标",rs1.getString("是否达标"));
//                    list.add(map);
//                }
//                connection.commit();
//                rs1.close();
//                rs1 = null;
//            }
//            int chaobiaoCount = 0;
//            List<Map<String,String>> waterFounctionList = new ArrayList<Map<String, String>>();
//            String waterFounctionName = null;
//            for(int i = 0 ; i < list.size() ; i++){
//                Map<String,String> map = list.get(i);
//                if(waterFounctionID.equals(map.get("waterFounctionID"))){
//                    if(waterFounctionName == null){
//                        waterFounctionName = map.get("waterFounctionName");
//                    }
//                    waterFounctionList.add(map);
//                }
//            }
//
//            for(int j = 0 ; j < waterFounctionList.size() ; j++){
//                if("1".equals(waterFounctionList.get(j).get("是否达标"))){
//                    chaobiaoCount++;
//                }
//            }
//            String sql1 = "select 河道断面_ID from mgmb_rel_断面与水体水功能区 where 水功能区_ID = ?";
//            ps = connection.prepareStatement(sql1);
//            ps.setString(1,waterFounctionID);
//            rs1 = ps.executeQuery();
//            List<String> dmIdList = new ArrayList<String>();
//            while (rs1.next()){
//                dmIdList.add(rs1.getString(1));
//            }
//            rs1.close();
//            ps.close();
//            String sql3 = "select t1.观测item_id, t2.sys_name, t1.观测值\n" +
//                    "  from szpj_biz_水质指标观测数据 t1, szpj_sys_指标项 t2\n" +
//                    " where t1.观测点_id = ?\n" +
//                    "   and to_char(t1.观测时间, 'yyyy-MM-dd') between ? and ?\n" +
//                    "   and t1.观测item_id = t2.item_id";
//            ps = connection.prepareStatement(sql3);
//            Set<String> itemIdSet = new HashSet<String>();
//            for(int i = 0 ; i < dmIdList.size() ; i++){
//                ps.setString(1,dmIdList.get(i));
//                ps.setString(2,Integer.valueOf(begin_year)+"-01-01");
//                ps.setString(3,Integer.valueOf(end_year)+"-12-31");
//                rs2 = ps.executeQuery();
//                while (rs2.next()){
//                    itemIdSet.add(rs2.getString("观测ITEM_ID"));
//                    Map<String,Object> map = new HashMap<String,Object>();
//                    map.put("itemId",rs2.getString("观测ITEM_ID"));
//                    map.put("itemName",rs2.getString("sys_name"));
//                    map.put("observeValue",rs2.getDouble("观测值"));
//                    dmList.add(map);
//                }
//            }
//
//            resultMap.put("waterFounctionID",waterFounctionID);
//            resultMap.put("waterFounctionName",waterFounctionName);
//            resultMap.put("超标次数",chaobiaoCount);
//            Iterator itemId = itemIdSet.iterator();
//            String sql2 = "select t2.sys_name, t1.value\n" +
//                    "  from szpj_sys_指标达标值 t1, szpj_sys_指标项 t2\n" +
//                    " where t1.item_id = t2.item_id\n" +
//                    "   and t1.level_id = '3'\n" +
//                    "   and t1.item_id = ?";
//
//            while (itemId.hasNext()){
//                int itemCount = 0;
//                double standardItemValue = 0;
//                String currentItemId = (String) itemId.next();
//                String currentItemName = null;
//                ps = connection.prepareStatement(sql2);
//                ps.setString(1,currentItemId);
//                rs2 = ps.executeQuery();
//                if(rs2.next()){
//                    standardItemValue = rs2.getDouble(2);
//                }
//                for(int i = 0 ; i < dmList.size() ; i++){
//                    Map map = dmList.get(i);
//                    if(currentItemId.equals(map.get("itemId"))){
//                        currentItemName = (String) map.get("itemName");
//                        double observeValue = (double) map.get("observeValue");
//                        if(observeValue > standardItemValue){
//                            itemCount++;
//                        }
//                    }
//                }
//                resultMap.put(currentItemName,itemCount);
//            }
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            try {
//                if(rs1 != null)
//                    rs1.close();
//                if(rs2 != null)
//                    rs2.close();
//                if(ps != null){
//                    ps.close();
//                }
//                if(call!= null)
//                    call.close();
//                if(connection != null)
//                    connection.close();
//            } catch (SQLException e) {
//                connection.rollback();
//                e.printStackTrace();
//                throw e;
//            }
//        }
//        return resultMap;
//    }

//    @Override
//    public List<Map<String, Object>> getDMSZPJCount2(String water_body_id, String waterFounctionID, String begin_year, String end_year) throws SQLException {
//        Connection connection  = null;
//        CallableStatement call = null;
//        ResultSet rs = null;
//        int count = 0 ;
//        //存放结果
//        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
//        //存放待整理数据
//        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
//        try {
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
//                call.setString(1,water_body_id);
//                call.setString(2,"year");
//                call.setString(3,waterFounctionID);
//                call.setString(4, i + "-01-01");
//                call.setString(5, i + "-12-31");
//                call.registerOutParameter(6, OracleTypes.CURSOR);
//                call.registerOutParameter(7, OracleTypes.CURSOR);
//                call.registerOutParameter(8, OracleTypes.CURSOR);
//                call.execute();
//                rs = (ResultSet) call.getObject(7);
//                while (rs.next()){
//                    Map<String,String> map = new HashMap<String,String>();
//                    map.put("dmID",rs.getString("评价对象_ID"));
//                    map.put("dmName",rs.getString("WR_SD_NM"));
//                    map.put("水质状况",rs.getString("水质状况"));
//                    list.add(map);
//                }
//                connection.commit();
//                rs.close();
//                rs = null;
//            }
//
//            Set<String> dmNames = new HashSet<String>();
//
//            for(int i = 0 ; i < list.size() ; i++){
//                String dmName = list.get(i).get("dmName");
//                dmNames.add(dmName);
//
//            }
//            Iterator dmName = dmNames.iterator();
//            while (dmName.hasNext()){
//                String currentdmID = null;
//                String currentdmName = (String) dmName.next();
//                List<Map<String,String>> dmList = new ArrayList<Map<String, String>>();
//                for(int j = 0 ; j < list.size() ; j++){
//                    String name =  list.get(j).get("dmName");
//                    if(name.equals(currentdmName)){
//                        if(currentdmID == null){
//                            currentdmID = list.get(j).get("dmID");
//                        }
//                        dmList.add(list.get(j));
//                    }
//                }
//                int countStep1 = 0;//优
//                int countStep2 = 0;//良好
//                int countStep3 = 0;//轻度污染
//                int countStep4 = 0;//中度污染
//                int countStep5 = 0;//重度污染
//
//                for(int j = 0 ; j < dmList.size() ; j++){
//                    String dmCondition = (String) dmList.get(j).get("水质状况");
//                    if("优".equals(dmCondition)){
//                        countStep1++;
//                    }
//                    if("良好".equals(dmCondition)){
//                        countStep2++;
//                    }
//                    if("轻度污染".equals(dmCondition)){
//                        countStep3++;
//                    }
//                    if("中度污染".equals(dmCondition)){
//                        countStep4++;
//                    }
//                    if("重度污染".equals(dmCondition)){
//                        countStep5++;
//                    }
//                }
//                count = countStep1 + countStep2 + countStep3 + countStep4 + countStep5;
//                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//                resultMap.put("dmID",currentdmID);
//                resultMap.put("dmName",currentdmName);
//                resultMap.put("次数总计",count);
//                resultMap.put("优",countStep1);
//                resultMap.put("良好",countStep2);
//                resultMap.put("轻度污染",countStep3);
//                resultMap.put("中度污染",countStep4);
//                resultMap.put("重度污染",countStep5);
//                resultList.add(resultMap);
//            }
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            if(rs != null)
//                rs.close();
//            if(call!= null)
//                call.close();
//            if(connection != null)
//                connection.close();
//        }
//        return resultList;
//    }

//    @Override
//    public List<Map<String, Object>> getDMSZPJCountDetail(String water_body_id, String dmID, String begin_year, String end_year) throws SQLException {
//        Connection connection  = null;
//        CallableStatement call = null;
//        ResultSet rs1 = null;
//        ResultSet rs2 = null;
//        //存放结果
//        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
//        //存放断面
//        String dmName = null;
//        //存放超标指标项
//        Set<String> itemName = new HashSet<String>();
//        //存放断面的超标项
//        List<Map<String , Object>> dmItemList = new ArrayList<Map<String, Object>>();
//        try {
//            connection = basicDataSource.getConnection();
//            connection.setAutoCommit(false);
//            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
//            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
//                call.setString(1,water_body_id);
//                call.setString(2,"year");
//                call.setString(3,dmID);
//                call.setString(4, i + "-01-01");
//                call.setString(5, i + "-12-31");
//                call.registerOutParameter(6, OracleTypes.CURSOR);
//                call.registerOutParameter(7, OracleTypes.CURSOR);
//                call.registerOutParameter(8, OracleTypes.CURSOR);
//                call.execute();
//                rs1 = (ResultSet) call.getObject(6);
//                rs2 = (ResultSet) call.getObject(8);
//                if (dmName == null) {
//                    if (rs1.next()){
//                        dmName = rs1.getString("WR_SD_NM");
//                    }
//                }
//                while (rs2.next()){
//                    if(rs2.getDouble("超标倍数")>0){
//                        itemName.add(rs2.getString("SYS_NAME"));
//                        Map<String,Object> map = new HashMap<String,Object>();
//                        map.put("dmID",dmID);
//                        map.put("itemName",rs2.getString("SYS_NAME"));
//                        map.put("超标倍数",rs2.getDouble("超标倍数"));
//                        dmItemList.add(map);
//                    }
//                }
//                try {
//                    connection.commit();
//                }catch (SQLException e){
//                    connection.rollback();
//                }
//                rs1.close();
//                rs2.close();
//                rs1 = null;
//                rs1 = null;
//            }
//            Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//            resultMap.put("dmID",dmID);
//            resultMap.put("dmName",dmName);
//            Iterator item = itemName.iterator();
//            while (item.hasNext()){
//                //存放当前断面当前超标项的内容
//                List list2 = new ArrayList();
//                String currentItem = (String) item.next();
//                for(int m = 0 ; m < dmItemList.size() ; m++){
//                    Map map = (Map) dmItemList.get(m);
//                    if(map.get("itemName").equals(currentItem)){
//                        list2.add(map);
//                    }
//                }
//                resultMap.put(currentItem,list2.size());
//            }
//            resultList.add(resultMap);
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            if(call!= null)
//                call.close();
//            if(connection != null)
//                connection.close();
//        }
//        return resultList;
//    }

    @Override
    public boolean deletePlanById(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql ="delete from sdlm_biz_方案定义表  where plan_id = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.execute();
            return true;
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return false;
    }

    @Override
    public boolean deleteInputFileById(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql ="delete from sdlm_biz_运算参数表  where 方案_id = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.execute();
            return true;
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return false;
    }
    @Override
    public boolean deleteInputParamsById(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql ="delete from sdlm_biz_方案条件设定表  where step_id in (select step_id from sdlm_biz_方案步骤定义 where plan_id = ?)";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.execute();
            return true;
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return false;
    }

    @Override
    public void deleteExist(String step_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql ="delete from sdlm_output_计算结果表 where step_id = ?";
        try {
            connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                ps.setString(1,step_id);
                ps.execute();
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public Map<String, String> getStartAndEndDate(String period_id, String searchType,Connection connection) throws SQLException {
        CallableStatement call = null;
        ResultSet rs = null;
        Map<String,String> dateMap = new HashMap<String,String>();

        Pattern pattern = Pattern.compile("(\\d{4})(-)(\\d{2})(-)(\\d{2})(~)(\\d{4})(-)(\\d{2})(-)(\\d{2})");
        Matcher matcher = pattern.matcher(period_id);
        if(matcher.find()){
            //符合yyyy-mm-dd~yyyy-mm-dd格式的日期
            String[] date = period_id.split("~");
            dateMap.put("startDate",date[0]);
            dateMap.put("endDate",date[1]);
        }else{
            //传过来的是周期ID
            try {
                connection = basicDataSource.getConnection();
                call = connection.prepareCall("{call SP_GETBEGENDDATE(?,?,?)}");
                call.setString(1, period_id);
                call.setString(2, StringUtils.isEmpty(period_id)?searchType:period_id);
                call.registerOutParameter(3, OracleTypes.CURSOR);
                call.execute();
                rs = (ResultSet) call.getObject(3);
                if (rs.next()){
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                    dateMap.put("startDate",sdf.format(rs.getDate(1)));
                    dateMap.put("endDate",sdf.format(rs.getDate(2)));
                }else{
                    throw new NullException("当前查询数据没有观测数据，无法查询，请重新查询");
                }
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (NullException e) {
                e.printStackTrace();
            } finally {
                if (rs != null){
                    rs.close();
                }
                if(call != null){
                    call.close();
                }
            }
        }
        return dateMap;
    }

    //判断水资源承载力污染物是否超标(行政区)
    public Map<String, Object> getPollutionExcessive(String water_body_id, String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
        //存放该行政区划范围内所有水功能区起止年份内化学需氧量和氨氮两个指标项的污染物入河量
        List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
        //存放标准定义为0的年份
        List<Integer> years = new ArrayList<Integer>();
        //存放行政区的水质达标状况结果
        Map resultMap = new LinkedHashMap();
        //根据传入的年份year，通过szpj_sys_污染物限排标准这个表找出计算水功能区污染物入河量的年份
        //存放计算水功能区污染物入河量的开始的年份
        int startYear = 2001;
        int endYear = Integer.parseInt(year);
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select distinct 年份 from szpj_sys_污染物限排标准 where 标准定义 = '0'";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            while (rs.next()){
                years.add(rs.getInt("年份"));
            }
            for(int i = 0 ; i < years.size() ; i++){
                if(endYear>years.get(i)){
                    startYear = years.get(i)+1;
                    break;
                }
            }
            rs.close();
            ps.close();
            String sql2 = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql2);
            ps.setInt(1, startYear);
            ps.setInt(2, endYear);
            ps.setString(3, xzq_id);
            ps.setString(4,water_body_id);
            ps.setInt(5, startYear);
            ps.setInt(6, endYear);
            ps.setString(7, xzq_id);
            ps.setString(8,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                list.add(map);
            }
            //将 currentYearTotalItemValueList 里边的化学需氧量和氨氮的入河量分别相加，和相对应的限排标准相比较
            String sql3 = "select t1.年份,t1.行政区划_ID,t2.sys_name,t1.年度累加限排总量,t1.水功能区数量最严格达标率,t1.水功能区长度最严格达标率 from szpj_sys_污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and 年份 = ?";
            ps = connection.prepareStatement(sql3);
            ps.setInt(1, Integer.parseInt(year));
            rs = ps.executeQuery();
            ResultSetMetaData metaData1 = rs.getMetaData();
            List<Map<String,Object>> xianPaiList = new ArrayList<Map<String,Object>>();
            while (rs.next()){
                Map<String,Object> map = new HashMap<String,Object>();
                for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                    String columnName = metaData1.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                xianPaiList.add(map);
            }
            double valueCOD = 0;
            double valueNH = 0;
            double xianPaiCOD = 0;
            double xianPaiNH = 0;
            String xzqName = null;
            String xzqID = null;
            for(int i = 0 ; i < xianPaiList.size() ; i++){
                Map map = xianPaiList.get(i);
                if("化学需氧量(COD)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                    BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                    xianPaiCOD = value.doubleValue();
                }
                if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                    BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                    xianPaiNH = value.doubleValue();
                }
            }
            boolean CODFlag = false;
            boolean NHFlag = false;
            for(int i = 0 ; i < list.size() ; i++){
                Map map = (Map) list.get(i);
                if(xzqID == null){
                    xzqID = (String) map.get("上游行政区_id");
                }
                if(xzqName == null){
                    xzqName = (String) map.get("行政区划名称");
                }
                if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                    BigDecimal value = (BigDecimal) map.get("污染物入河量");
                    valueCOD = valueCOD + value.doubleValue();
                    if(!CODFlag){
                        CODFlag = true;
                    }
                }
                if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                    BigDecimal value = (BigDecimal) map.get("污染物入河量");
                    valueNH = valueNH + value.doubleValue();
                    if(!NHFlag){
                        NHFlag = true;
                    }
                }
            }
            resultMap.put("年份",year);
            resultMap.put("水质达标状况","");
            if(xianPaiCOD>0&&CODFlag&&NHFlag){
                if(valueCOD<=xianPaiCOD&&valueNH<=xianPaiNH){
                    resultMap.put("水质达标状况","不超标");
                }else if(valueCOD>xianPaiCOD||valueNH>xianPaiNH){
                    resultMap.put("水质达标状况","超标");
                }
            }
        } catch (SQLException e) {
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
        return resultMap;
    }

    //判断水资源承载力污染物是否超标(河段)
    public Map<String, Object> getPollutionExcessive(String water_body_id, String xzq_id,String riverID,String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy");
        //存放该行政区划范围内所有水功能区起止年份内化学需氧量和氨氮两个指标项的污染物入河量
        List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();
        //存放标准定义为0的年份
        List<Integer> years = new ArrayList<Integer>();
        //存放行政区的水质达标状况结果
        Map resultMap = new LinkedHashMap();
        //根据传入的年份year，通过szpj_sys_污染物限排标准这个表找出计算水功能区污染物入河量的年份
        //存放计算水功能区污染物入河量的开始的年份
        int startYear = 2001;
        int endYear = Integer.parseInt(year);
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select distinct 年份 from szpj_sys_污染物限排标准 where 标准定义 = '0'";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            while (rs.next()){
                years.add(rs.getInt("年份"));
            }
            for(int i = 0 ; i < years.size() ; i++){
                if(endYear>years.get(i)){
                    startYear = years.get(i)+1;
                    break;
                }
            }
            rs.close();
            ps.close();
            String sql2 = "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id = ?\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n" +
                    "union\n" +
                    "select t4.上游行政区_id,\n" +
                    "       t5.行政区划名称,\n" +
                    "       t1.监测日期,\n" +
                    "       t4.水功能区_id,\n" +
                    "       t9.wfz_nm,\n" +
                    "       t6.item_id,\n" +
                    "       t6.sys_name,\n" +
                    "       t1.污染物入河量,\n" +
                    "       t7.水体_ID,\n" +
                    "       t8.水体名称\n" +
                    "  from szpj_biz_排污口污染物入河量 t1,\n" +
                    "       mgmb_入河排污口信息简表     t2,\n" +
                    "       wr_pdo_b                    t3,\n" +
                    "       mgmb_水功能区信息简表       t4,\n" +
                    "       v_行政区划                  t5,\n" +
                    "       szpj_sys_指标项             t6,\n" +
                    "       mgmb_rel_水功能区与水体     t7,\n" +
                    "       v_水体                      t8,\n" +
                    "       wr_wfz_b                    t9\n" +
                    " where t1.排污口_id = t2.入河排污口_id\n" +
                    "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                    "   and t2.wr_sd_code = t3.pdo_cd\n" +
                    "   and t3.wfz_cd = t4.wr_sd_code\n" +
                    "   and t4.上游行政区_id = t5.行政区_id\n" +
                    "   and t4.上游行政区_id = ?\n" +
                    "   and t6.item_id = t1.污染物指标_id\n" +
                    "   and t7.水功能区_id = t4.水功能区_id\n" +
                    "   and t7.水体_id in\n" +
                    "       (select 支流_ID\n" +
                    "          from zdst_rel_干支流关系表\n" +
                    "         where 干流_id = ?)\n" +
                    "   and t7.水体_id = t8.水体_id\n" +
                    "   and t4.wr_sd_code = t9.wfz_cd\n";
            ps = connection.prepareStatement(sql2);
            ps.setInt(1, startYear);
            ps.setInt(2, endYear);
            ps.setString(3, xzq_id);
            ps.setString(4,water_body_id);
            ps.setInt(5, startYear);
            ps.setInt(6, endYear);
            ps.setString(7, xzq_id);
            ps.setString(8,water_body_id);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                for(int i = 0 ; i < metaData.getColumnCount() ; i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                list.add(map);
            }
            //将 currentYearTotalItemValueList 里边的化学需氧量和氨氮的入河量分别相加，和相对应的限排标准相比较
            String sql3 = "select t1.年份,t1.行政区划_ID,t2.sys_name,t1.年度累加限排总量,t1.水功能区数量最严格达标率,t1.水功能区长度最严格达标率 from szpj_sys_污染物限排标准 t1,szpj_sys_指标项 t2 where t1.污染物指标_id = t2.item_id and 年份 = ?";
            ps = connection.prepareStatement(sql3);
            ps.setInt(1, Integer.parseInt(year));
            rs = ps.executeQuery();
            ResultSetMetaData metaData1 = rs.getMetaData();
            List<Map<String,Object>> xianPaiList = new ArrayList<Map<String,Object>>();
            while (rs.next()){
                Map<String,Object> map = new HashMap<String,Object>();
                for(int i = 0 ; i < metaData1.getColumnCount() ; i++){
                    String columnName = metaData1.getColumnName(i+1);
                    map.put(columnName,rs.getObject(columnName));
                }
                xianPaiList.add(map);
            }
            double valueCOD = 0;
            double valueNH = 0;
            double xianPaiCOD = 0;
            double xianPaiNH = 0;
            String rID = null;
            String rName = null;
            for(int i = 0 ; i < xianPaiList.size() ; i++){
                Map map = xianPaiList.get(i);
                if("化学需氧量(COD)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                    BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                    xianPaiCOD = value.doubleValue();
                }
                if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))&&xzq_id.equals(map.get("行政区划_ID"))){
                    BigDecimal value = (BigDecimal) map.get("年度累加限排总量");
                    xianPaiNH = value.doubleValue();
                }
            }
            List riverList = new ArrayList();
            for(int i = 0 ; i < list.size() ; i++){
                if(list.get(i).get("水体_ID").equals(riverID)){
                    riverList.add(list.get(i));
                }
            }
            boolean CODFlag = false;
            boolean NHFlag = false;
            for(int i = 0 ; i < riverList.size() ; i++){
                Map map = (Map) riverList.get(i);
                if(rID == null){
                    rID = (String) map.get("水体_ID");
                }
                if(rName == null){
                    rName = (String) map.get("水体名称");
                }
                if("化学需氧量(COD)".equals(map.get("SYS_NAME"))){
                    BigDecimal value = (BigDecimal) map.get("污染物入河量");
                    valueCOD = valueCOD + value.doubleValue();
                    if(!CODFlag){
                        CODFlag = true;
                    }
                }
                if("氨氮(NH3-N)".equals(map.get("SYS_NAME"))){
                    BigDecimal value = (BigDecimal) map.get("污染物入河量");
                    valueNH = valueNH + value.doubleValue();
                    if(!NHFlag){
                        NHFlag = true;
                    }
                }
            }
//            resultMap.put("水体_ID",rID);
//            resultMap.put("水体名称",rName);
            resultMap.put("年份",year);
            resultMap.put("水质达标状况","");
            if(xianPaiCOD>0&&CODFlag&&NHFlag){
                if(valueCOD<xianPaiCOD&&valueNH<xianPaiNH){
                    resultMap.put("水质达标状况","不超标");
                }else if(valueCOD>xianPaiCOD||valueNH>xianPaiNH){
                    resultMap.put("水质达标状况","超标");
                }
            }
        } catch (SQLException e) {
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
        return resultMap;
    }
    @Override
    public boolean saveSdlmxResult1(List<String[]> list, String[] strData1, Date date, int countNum, String step_id) throws Exception {

        String objNames[] = {"计算断面号","起点长度","流量","最低高程","断面水位","断面水深","断面流速","fr数","面积","宽度"};
        List<String> objsName = new ArrayList<String>();
        Collections.addAll(objsName, objNames);
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<Integer,String> map = new HashMap<Integer,String>();
        try {
            connection=basicDataSource.getConnection();
            connection.setAutoCommit(false);
//            String sql = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = '73476806f527419686d18gbca8485689'";
            String checkSql = "select  计算项目_id,计算项目名称 from SDLM_SYS_计算结果项目表 where 模型类型_id = '0' and  计算项目名称 = ?";
            String sql3 = "insert into SDLM_OUTPUT_计算结果表 (sn_id,step_id,result_date,计算断面号,计算项目_id,计算结果_value) values(?,?,?,?,?,?)";
//            String step_id = null;
//            ps = connection.prepareStatement(sql);
//            ps.setString(1,plan_id);
//            rs = ps.executeQuery();
//            if(rs.next()){
//                step_id = rs.getString(1);
//            }else {
//                return false;
//            }
//             查询并插入 SDLM_SYS_计算结果项目表
            ps = connection.prepareStatement(checkSql);
            for(int i =1 ;i<objNames.length;i++){
                ps.setString(1,objNames[i]);
                rs = ps.executeQuery();
                while(rs.next()){
                    map.put(objsName.indexOf(rs.getString(2)), rs.getString(1));
                }
            }
            String []  str= null;
            ps = connection.prepareStatement(sql3);
            //插入 SDLM_OUTPUT_计算结果表
            //插入 hydro
            for(int i=0;i<countNum;i++){
                for(int key=1;key<objNames.length-2;key++){
                    ps.setString(1, Md5Util.getUUID());
                    ps.setString(2, step_id);
                    ps.setDate(3, new java.sql.Date(date.getTime()));
                    str = list.get(i);
                    ps.setInt(4,Integer.valueOf(str[0]));
                    ps.setString(5,map.get(key));
                    ps.setString(6, str[key]);
                    ps.addBatch();
                }
            }
//            ps.executeBatch();
//            ps = connection.prepareStatement(sql3);
            //插入OUTQAB
            for(int i=0;i<countNum;i++){
                    ps.setString(1, Md5Util.getUUID());
                    ps.setString(2, step_id);
                    ps.setDate(3, new java.sql.Date(date.getTime()));
                    str = list.get(i);
                    ps.setInt(4,Integer.valueOf(str[0]));
                    ps.setString(5,map.get(8));
                    ps.setString(6, strData1[countNum + i]);
                    ps.addBatch();
                    ps.setString(1, Md5Util.getUUID());
                    ps.setString(2, step_id);
                    ps.setDate(3, new java.sql.Date(date.getTime()));
                    str = list.get(i);
                    ps.setInt(4,Integer.valueOf(str[0]));
                    ps.setString(5,map.get(9));
                    ps.setString(6, strData1[countNum * 2 + i]);
                    ps.addBatch();
            }
            System.out.println("保存数据...............");
            ps.executeBatch();
            connection.commit();
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
        return true;
    }
    @Override
    public boolean saveSzmxResultFile(File f,String step_id) throws Exception {

        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection=basicDataSource.getConnection();
            connection.setAutoCommit(false);

            String sql2 = "insert into SDLM_ATT_计算结果文件 values(?,?,empty_blob())";
            //保存两个生成文件
            ps = connection.prepareStatement(sql2);
            String uuid1 = Md5Util.getUUID();
            ps.setString(1, uuid1);
            ps.setString(2, f.getName());
            ps.executeUpdate();
            DBUtil.updateBlob(f, connection, "SDLM_ATT_计算结果文件", "step_id", uuid1,"附件_obj");
            connection.commit();
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return true;
    }
    @Override
    public boolean saveSzmxResult(List<String[]> list, String[] strData1, Date date, int countNum) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection=basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql = "insert into SDLM_OUTPUT_计算结果表 values (?,?,?,?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql);
            String []  str= null;
            for(int i=0;i<countNum;i++){
                ps.setString(1, Md5Util.getUUID());
                ps.setDate(2, new java.sql.Date(date.getTime()));
                str = list.get(i);
                ps.setInt(3, Integer.valueOf(str[0]));
                ps.setString(4, str[1]);
                ps.setString(5, str[2]);
                ps.setString(6, str[3]);
                ps.setString(7, str[4]);
                ps.setString(8, str[5]);
                ps.setString(9, str[6]);
                ps.setString(10, str[7]);
                ps.setString(11, strData1[countNum + i]);
                ps.setString(12, strData1[countNum * 2 + i]);
                ps.addBatch();
            }
            ps.executeBatch();
            connection.commit();
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return true;
    }
//---------------------------------------------------水质评价（getSZPJRiverLevel）
//存放断面（水功能区）的水质信息   szpj_sys_断面水质类别定义
    public  List<Map<String,Object>> getSortInfo(Connection connection) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> sortInfo = new ArrayList<Map<String,Object>>();
        String sql1 = "select * from szpj_sys_断面水质类别定义";
        try {
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> sortMap = new HashMap<String,Object>();
                sortMap.put("level",rs.getInt("LEVEL_ID"));
                sortMap.put("LEVEL_ID",rs.getString("水质状况"));
                sortMap.put("水质状况",rs.getString("水质状况"));
                sortMap.put("表征颜色",rs.getString("COLOR_VALUE"));
                sortInfo.add(sortMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return sortInfo;
    }
    @Override
    public List<Map<String, String>> getSZPJWaterFounctionInRiver(String riverId,String startDate,String endDate) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List<Map<String, String>> waterFounctionList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            //存放断面（水功能区）的水质信息   szpj_sys_断面水质类别定义
            List<Map<String,Object>> sortInfo = getSortInfo(connection);
            String sql2 = "select t1.水功能区_id,\n" +
                    "       t1.水功能区名称,\n" +
                    "       t2.super_obj_id,\n" +
                    "       t1.水质目标id,\n" +
                    "       t1.水质目标名称,\n" +
                    "       t3.等级名称\n" +
                    "  from v_水功能区信息 t1, v_水资源对象 t2, mgmb_sys_水功能区分级表 t3\n" +
                    " where t1.水功能区_id = t2.obj_id\n" +
                    "   and t1.分级id = t3.sn_id\n" +
                    "   and t1.水体id = ?\n";

            ps = connection.prepareStatement(sql2);
            ps.setString(1,riverId);
            rs = ps.executeQuery();
            while (rs.next()){
                //
                Map<String, String> waterFounctionMap = new LinkedHashMap<String,String>();
                waterFounctionMap.put("评价方法","");
                waterFounctionMap.put("RIVER_ID",riverId);
                waterFounctionMap.put("评价对象_ID",rs.getString(1));
                waterFounctionMap.put("WR_SD_NM",rs.getString(2));
                waterFounctionMap.put("SUPER_OBJ_ID",rs.getString(3));
                String waterFounctionGrade = rs.getString(6);
                //LEVEL_ID
                //水质状况
                //表征颜色
                int level = getSZPJWaterFounction(connection,rs.getString(1),waterFounctionGrade,startDate,endDate);
                if(level == 0){
                    //若为0，则表示
                    waterFounctionMap.put("LEVEL_ID", "");
                    waterFounctionMap.put("水质状况", "");
                    waterFounctionMap.put("表征颜色", "");
                }else {
                    for(int i = 0 ; i < sortInfo.size() ; i++){
                        Map<String,Object> map = sortInfo.get(i);
                        if((int)map.get("level") == level){
                            waterFounctionMap.put("LEVEL_ID", (String) map.get("LEVEL_ID"));
                            waterFounctionMap.put("水质状况", (String) map.get("水质状况"));
                            waterFounctionMap.put("表征颜色", (String) map.get("表征颜色"));
                        }
                    }
                }
                waterFounctionMap.put("水质目标",rs.getString(5)==null?"":rs.getString(5));
                //是否达标
                //达标颜色
                if(StringUtils.isNotEmpty(rs.getString(4)) && level > 0){
                    if(level <= Integer.valueOf(rs.getString(4))){
                        waterFounctionMap.put("是否达标", "0");//达标
                        waterFounctionMap.put("达标颜色", "#00FF00");
                    }else{
                        waterFounctionMap.put("是否达标", "1");//不达标
                        waterFounctionMap.put("达标颜色", "#FF0000");
                    }
                }else{
                    waterFounctionMap.put("是否达标", "-1");//水功能区无水质目标，无法判断达标情况
                    waterFounctionMap.put("达标颜色", "");
                }


                waterFounctionList.add(waterFounctionMap);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return waterFounctionList;
    }



    //某水功能区内的水质评价
    public int getSZPJWaterFounction(Connection connection , String waterFounctionId,String waterFounctionGrade,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        int level = 0;

        String sql2 = " SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "           and s.水功能区_id = ?";
        try {

            //存放该水功能区里面相关的断面和水质站
            List<String> relatedDmList = getDmOrSzIDList(connection,waterFounctionId);

            //根据relatedDmList里面的数据的个数分类，如果是只有一条数据，则按照水利部版的水功能区水质评价方法的第一个评价
            if(relatedDmList.size()==1){
                level = getSZPJDmOrSz(connection,relatedDmList,startDate,endDate);
            }else if(relatedDmList.size() > 1){
                if("缓冲区".equals(waterFounctionGrade)){
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1,waterFounctionId);
                    rs = ps.executeQuery();
                    if (rs.next()){
                        List<String> dmIdList = new ArrayList<String>();
                        dmIdList.add(rs.getString(2));
                        level = getSZPJDmOrSz(connection,dmIdList,startDate,endDate);
                    }
                }else if("饮用水源区".equals(waterFounctionGrade)){
                    for(int k = 0 ; k < relatedDmList.size() ; k++){
                        List<String> dmIdList = new ArrayList<String>();
                        dmIdList.add(relatedDmList.get(k));
                        int dmLevel = getSZPJDmOrSz(connection,dmIdList,startDate,endDate);
                        if(level < dmLevel){
                            level = dmLevel;
                        }
                    }
                }else{
                    level = getSZPJDmOrSz(connection,relatedDmList,startDate,endDate);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return level;
    }

    //一个或者多个断面或者水质站的水质评价
    public int getSZPJDmOrSz(Connection connection , List<String> observeId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        int level = 0;
        String sql = "select t.item_id, t.level_id, t.value, t.remark\n" +
                "  from szpj_sys_指标达标值 t\n" +
                " where t.item_id = ?\n" +
                " order by t.level_id\n";
        try {
            //存放该断面的各个指标值（不同时段会有指标值，所以各个指标值应该按照时间平均之后，再与指标达标值比较进行水质评价）
            List<Map<String,Object>> itemList = getDmOrSzItemList(connection,observeId,startDate,endDate);

            ps = connection.prepareStatement(sql);
            for(int j = 0 ; j < itemList.size() ; j++){
                Map<String,Object> map = itemList.get(j);
                String itemId = (String) map.get("itemId");
                double itemValue = (double) map.get("itemAvgValue");
                ps.setString(1,itemId);
                rs = ps.executeQuery();
                while (rs.next()){
                    if(">=".equals(rs.getString(4)) && itemValue >= rs.getDouble(3)){
                        if(level < rs.getInt(2)){
                            level = rs.getInt(2);
                        }
                        break;
                    }
                    if(">=".equals(rs.getString(4)) && rs.getInt(2) == 5 && itemValue < rs.getDouble(3)){
                        level = 6;
                    }
                    if("<=".equals(rs.getString(4)) && itemValue <= rs.getDouble(3)){
                        if(level < rs.getInt(2)){
                            level = rs.getInt(2);
                        }
                        break;
                    }
                    if("<=".equals(rs.getString(4)) && rs.getInt(2) == 5 && itemValue > rs.getDouble(3)){
                        level = 6;
                    }
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }

        return level;
    }

    //获取一个或者多个断面、水质站的监测信息（真实监测数据）
    public List<Map<String,Object>> getDmOrSzTrueItemList(Connection connection , List<String> observeId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        //存放该断面的各个指标值（不同时段会有指标值，所以各个指标值应该按照时间平均之后，再与指标达标值比较进行水质评价）
        List<Map<String,Object>> itemList = new ArrayList<Map<String,Object>>();
        String sql1 = "select t1.观测点_id,t1.观测时间, t1.观测item_id, t2.sys_name, t1.观测值\n" +
                "  from szpj_biz_水质指标观测数据 t1, szpj_sys_指标项 t2\n" +
                " where t1.观测item_id = t2.item_id\n" +
                "   and t2.domain_id = '1'\n" +
                "   and t2.itype_id = '1'\n" +
                "   and t1.观测点_id = ?\n" +
                "   and t1.观测时间 >= to_date(?, 'yyyy-MM-dd')\n" +
                "   and t1.观测时间 <= to_date(?, 'yyyy-MM-dd')";
        try {
            ps = connection.prepareStatement(sql1);
            List<Map<String,Object>> tempList = new ArrayList<Map<String,Object>>();
            Set<String> dateSet = new HashSet<String>();
            for(int i = 0 ; i < observeId.size() ; i++){
                ps.setString(1,observeId.get(i));
                ps.setString(2,startDate);
                ps.setString(3,endDate);
                rs = ps.executeQuery();
                while (rs.next()){
                    if("化学需氧量(COD)".equals(rs.getString(4))||"氨氮(NH3-N)".equals(rs.getString(4))){
                        Map<String,Object> map = new HashMap<String,Object>();
                        map.put("observeDate",sdf.format(rs.getDate(2)));
                        map.put("itemId",rs.getString(3));
                        map.put("itemName",rs.getString(4));
                        map.put("itemValue",rs.getDouble(5));
                        dateSet.add(sdf.format(rs.getDate(2)));
                        tempList.add(map);
                    }
                }
            }
            Iterator date = dateSet.iterator();

            while (date.hasNext()){

                String currentDate = (String) date.next();
                double CODValue = 0;
                double NHValue = 0;
                int CODCount = 0;
                int NHCount = 0;
                boolean CODFlag = false;
                boolean NHFlag = false;
                for(int i = 0 ; i < tempList.size() ; i++){
                    Map<String,Object> map = tempList.get(i);
                    if(currentDate.equals(map.get("observeDate"))){
                        if("化学需氧量(COD)".equals(map.get("itemName"))){
                            CODCount++;
                            CODFlag = true;
                            CODValue += (double) map.get("itemValue");
                        }
                        if("氨氮(NH3-N)".equals(map.get("itemName"))){
                            NHCount++;
                            NHFlag = true;
                            NHValue += (double) map.get("itemValue");
                        }
                    }
                }

                if(CODFlag){
                    Map<String,Object> itemMap = new HashMap<String,Object>();
                    itemMap.put("observeDate",currentDate);
                    itemMap.put("化学需氧量(COD)",CODValue/CODCount);
                    itemList.add(itemMap);
                }
                if(NHFlag){
                    Map<String,Object> itemMap = new HashMap<String,Object>();
                    itemMap.put("observeDate",currentDate);
                    itemMap.put("氨氮(NH3-N)",NHValue/NHCount);
                    itemList.add(itemMap);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return itemList;
    }

//获取一个或多个断面或者水质站的监测信息（平均之后的）
    public List<Map<String,Object>> getDmOrSzItemList(Connection connection , List<String> observeId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放该断面的各个指标值（不同时段会有指标值，所以各个指标值应该按照时间平均之后，再与指标达标值比较进行水质评价）
        List<Map<String,Object>> itemList = new ArrayList<Map<String,Object>>();
        String sql1 = "select t1.观测点_id, t1.观测item_id, t2.sys_name, t1.观测值\n" +
                "  from szpj_biz_水质指标观测数据 t1, szpj_sys_指标项 t2\n" +
                " where t1.观测item_id = t2.item_id\n" +
                "   and t2.domain_id = '1'\n" +
                "   and t2.itype_id = '1'\n" +
                "   and t1.观测点_id = ?\n" +
                "   and t1.观测时间 >= to_date(?, 'yyyy-MM-dd')\n" +
                "   and t1.观测时间 <= to_date(?, 'yyyy-MM-dd')";
        try {
            ps = connection.prepareStatement(sql1);
            List<Map<String,Object>> dmOrSzList = new ArrayList<Map<String,Object>>();
            Set<String> idSet = new HashSet<String>();
            for(int i = 0 ; i < observeId.size() ; i++){
                ps.setString(1,observeId.get(i));
                ps.setString(2,startDate);
                ps.setString(3,endDate);
                rs = ps.executeQuery();
                while (rs.next()){
                    Map<String,Object> map = new HashMap<String,Object>();
                    map.put("observeId",rs.getString(1));
                    map.put("itemId",rs.getString(2));
                    map.put("itemName",rs.getString(3));
                    map.put("itemValue",rs.getDouble(4));
                    idSet.add(rs.getString(2));
                    dmOrSzList.add(map);
                }
            }
            Iterator id = idSet.iterator();

            while (id.hasNext()){
                Map<String,Object> itemMap = new HashMap<String,Object>();
                String currentId = (String) id.next();
                int count = 0 ;
                double sum = 0;
                String itemName = null;
                for(int i = 0 ; i < dmOrSzList.size() ; i++){
                    Map<String,Object> map = dmOrSzList.get(i);
                    if(currentId.equals(map.get("itemId"))){
                        count++;
                        sum = sum + (double)map.get("itemValue");
                        if(itemName == null){
                            itemName = (String) map.get("itemName");
                        }
                    }
                }
                if("水温(℃)".equals(itemName) || "溶解氧".equals(itemName) || "PH值(无量纲)".equals(itemName)){
                    continue;
                }
                double avg = sum/count;
                itemMap.put("itemId",currentId);
                itemMap.put("itemName",itemName);
                itemMap.put("itemAvgValue",avg);
                itemMap.put("observeCount",count);
                itemList.add(itemMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return itemList;
    }

    @Override
    public List<Map<String, Object>> getSZPJRiverLevel(String water_body_id, String searchType, String period_id, String obj_ids) throws Exception {
        Connection connection = null;
        CallableStatement call = null;

        ResultSet rs1 = null;

        List<Map<String,Object>> list = new ArrayList<Map<String,Object>>();

        Map<String,Object> map1 = null;

        try {
            connection = basicDataSource.getConnection();
            Map<String,String> dateMap = getStartAndEndDate(period_id,searchType,connection);
            String startDate = dateMap.get("startDate");
            String endDate = dateMap.get("endDate");
            connection.setAutoCommit(false);
            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
            call.setString(1,water_body_id);
            call.setString(2, searchType);
            call.setString(3, obj_ids);
            call.setString(4,startDate);
            call.setString(5, endDate);
            call.registerOutParameter(6, OracleTypes.CURSOR);
            call.registerOutParameter(7, OracleTypes.CURSOR);
            call.registerOutParameter(8, OracleTypes.CURSOR);
            call.execute();
            rs1 = (ResultSet) call.getObject(6);
            ResultSetMetaData metaData1 = rs1.getMetaData();
            //河流信息（多条）
            while (rs1.next()){
                map1 = new LinkedHashMap<String,Object>();
                for(int i = 0; i < metaData1.getColumnCount();i++){
                    String columnName = metaData1.getColumnName(i+1);
                    map1.put(columnName, rs1.getString(columnName));
                }
                //根据河流的评价对象_id找到相对应的详细信息   list3
                List<Map<String,String>> waterFounctionList = getSZPJWaterFounctionInRiver(rs1.getString("评价对象_ID"),startDate,endDate);
                map1.put("waterFunList", waterFounctionList);
                list.add(map1);
            }
            return list;
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                connection.commit();
                if(rs1 != null){
                    rs1.close();
                }
                if(call != null){
                    call.close();
                }
                if(connection != null){
                    connection.close();
                }
            } catch (SQLException e) {
                connection.rollback();
                e.printStackTrace();
                throw e;
            }
        }
    }

    @Override
    public List<Map<Object,Object>> getTarget(String userId, String targetIds,String type ,String water_body_id,String searchType,String period_id) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        PreparedStatement ps = null;
        PreparedStatement ps1 = null;
        PreparedStatement ps2 = null;
        ResultSet rs = null;
        ResultSet rs1 = null;
        ResultSet rs2 = null;
        String startDate = null;
        String endDate = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        SimpleDateFormat sdf1 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        List list = new ArrayList();
        //存放某一个断面最新时间的水文观测数据
//        List<Map<Object,Object>> swList = new ArrayList<Map<Object,Object>>();
        Map<Object,Object> swmap = null;
        //存放某一个断面最新时间的水质指标观测数据
        Map<Object,Object> szMap = null;
        try {
            connection = basicDataSource.getConnection();
            //先得到起止日期
            if(StringUtils.isEmpty(period_id)){
                String sql = "select 观测时间 from szpj_biz_水质指标观测数据 where 观测点_ID = ? order by 观测时间 desc";
                ps = connection.prepareStatement(sql);
                ps.setString(1, targetIds);
                rs = ps.executeQuery();
                if(rs.next()){
                    startDate = sdf1.format((rs.getTimestamp("观测时间")));
//                    endDate = startDate;
                }
                if(startDate != null){
                    String sql1 = "select t1.观测点_id,t3.rb_nm 断面名称,t1.观测时间,t1.水位,t1.流量,t1.水温,t1.气温 from szpj_biz_水文观测数据 t1,mgmb_河道断面信息简表 t2,wr_rb_b t3 where t1.观测时间 = to_date(?,'yyyy-mm-dd hh24:mi:ss') and t1.观测点_ID = t2.河道断面_ID and t3.rb_cd = t2.wr_sd_code and t1.观测点_id =?";
                    ps1 = connection.prepareStatement(sql1);
                    ps1.setString(1,startDate);
                    ps1.setString(2,targetIds);
                    rs1 = ps1.executeQuery();
                    if(rs1.next()){
                        ResultSetMetaData metaData1 = rs1.getMetaData();
                        swmap = new LinkedHashMap<Object,Object>();
                        for(int i = 0; i < metaData1.getColumnCount(); i++){
                            String columnName = metaData1.getColumnName(i+1);
                            if(rs1.getObject(columnName) != null){
                                swmap.put(columnName,rs1.getObject(columnName));
                            }
                        }
//                        swList.add(swmap);
//                        list.add(swmap);
                    }
                    if(swmap!=null){
//                        for(int size = 0; size < swList.size(); size++){
                        List<Map<Object,Object>> szList = new ArrayList<Map<Object,Object>>();
                        String sql2 = "select t2.sys_name,t1.观测值,t1.取样位置 from szpj_biz_水质指标观测数据 t1,szpj_sys_指标项 t2 where 观测时间 = to_date(?,'yyyy-mm-dd hh24:mi:ss') and t1.观测item_id = t2.item_id and t1.观测点_id = ?";
                        ps2 = connection.prepareStatement(sql2);
                        ps2.setString(1,startDate);
                        ps2.setString(2, (String) swmap.get("观测点_ID"));
                        rs2 = ps2.executeQuery();
                        ResultSetMetaData metaData2 = rs2.getMetaData();
                        Set<Object> columnNames = new HashSet<Object>();
                        while (rs2.next()){
                            szMap = new LinkedHashMap<Object,Object>();
                            for(int i = 0; i < metaData2.getColumnCount(); i++){
                                String columnName = metaData2.getColumnName(i+1);
                                szMap.put(columnName, rs2.getObject(columnName) != null ? rs2.getObject(columnName) : "");
                                if(rs2.getString(columnName) != null&&columnName.equals("SYS_NAME")){
                                    columnNames.add(rs2.getObject(columnName));
                                }
                            }
                            szList.add(szMap);
                        }
                        //处理指标项相同的观测值
                        Iterator name = columnNames.iterator();
                        Map map3 = new LinkedHashMap();
                        swmap.remove("观测点_ID");
                        swmap.remove("断面名称");
                        String time = sdf.format((java.util.Date)swmap.get("观测时间"));
                        swmap.put("观测时间", time);
                        map3.putAll(swmap);
                        while (name.hasNext()){
                            List list1 = new ArrayList();
                            String currentName = (String) name.next();
                            for(int i = 0; i < szList.size();i++){
                                Map map1 = szList.get(i);
                                //如果有指标相同的项就存放到list1中
                                if(map1.get("SYS_NAME").equals(currentName)){
                                    list1.add(map1);
                                }
                            }
                            //如果list1的大于0，说明有相同的指标项
                            if(list1.size()>0){
                                double avgGcValue = 0;
                                double gcValue = 0;
                                int count = list1.size();
                                for(int j = 0 ; j < list1.size();j++){
                                    Map map2 = (Map) list1.get(j);
                                    BigDecimal value = (BigDecimal) map2.get("观测值");
                                    gcValue = gcValue + value.doubleValue();
                                }
                                avgGcValue = gcValue/count;
                                map3.put(currentName,avgGcValue);
                            }//else{
//                                    Map map4 = (Map)list1.get(0);
//                                    map3.put(currentName,map4.get("观测值"));
//                                }
                        }
                        list.add(map3);
//                        }
                    }
                }
            }else{
                Map<String,String> dateMap = getStartAndEndDate(period_id,searchType,connection);
                startDate = dateMap.get("startDate");
                endDate = dateMap.get("endDate");
                //得到日期后，查询断面的监测数据
                List<String> dmId = new ArrayList<String>();
                dmId.add(targetIds);
                List<Map<String,Object>> itemList = getDmOrSzItemList(connection,dmId,startDate,endDate);

                String sql1 = "select t1.item_id, t1.level_id, t1.value, t1.remark,t2.水质状况\n" +
                        "  from szpj_sys_指标达标值 t1, szpj_sys_断面水质类别定义 t2\n" +
                        " where t1.item_id = ?\n" +
                        "   and t1.level_id = t2.level_id\n" +
                        " order by t1.level_id";
                String sql2 = "select t.item_id, t.level_id, t.value, t.remark\n" +
                        "  from szpj_sys_指标达标值 t\n" +
                        " where t.item_id = ?\n" +
                        "   and t.level_id = 3\n";
                ps = connection.prepareStatement(sql1);
                ps1 = connection.prepareStatement(sql2);
                for(int i = 0 ; i < itemList.size() ; i++){
                    Map<String,Object> map = itemList.get(i);
//                    if("水温(℃)".equals(map.get("itemName")) || "溶解氧".equals(map.get("itemName")) || "PH值(无量纲)".equals(map.get("itemName"))){
//                        continue;
//                    }
                    Map<Object,Object> resultMap = new HashMap<Object,Object>();
                    ps.setString(1, (String) map.get("itemId"));
                    double observeValue = (double) map.get("itemAvgValue");
                    resultMap.put("指标ID",map.get("itemId"));
                    resultMap.put("测量值",new DecimalFormat("0.#####").format(observeValue));

                    resultMap.put("指标名称",map.get("itemName"));
                    ps1.setString(1,(String) map.get("itemId"));
                    rs1 = ps1.executeQuery();
                    double excessValue = 0;
                    if (rs1.next()){
                        //断面或者水质站的当前指标项的超标倍数
                        if(rs1.getString(3) != null){
                            double standardValue = rs1.getDouble(3);
                            resultMap.put("III类标准限值",new DecimalFormat("0.#####").format(standardValue));
                            excessValue = (observeValue-standardValue)/standardValue;
                            resultMap.put("超标情况",((double)Math.round(excessValue*100)/100)>0?(double)Math.round(excessValue*100)/100:"");
                        }else{
                            resultMap.put("III类标准限值","");
                            resultMap.put("超标情况","");
                        }
                    }else{
                        resultMap.put("III类标准限值","");
                        resultMap.put("超标情况","");
                    }
                    rs = ps.executeQuery();
                    while (rs.next()){
                        if(">=".equals(rs.getString(4)) && observeValue >= rs.getDouble(3)){
                            resultMap.put("评价结果",rs.getString(5));
                            break;
                        }
                        if(">=".equals(rs.getString(4)) && rs.getInt(2) == 5 && observeValue < rs.getDouble(3)){
                            resultMap.put("评价结果","劣V 类水质");
                            break;
                        }
                        if("<=".equals(rs.getString(4)) && observeValue <= rs.getDouble(3)){
                            resultMap.put("评价结果",rs.getString(5));
                            break;
                        }
                        if("<=".equals(rs.getString(4)) && rs.getInt(2) == 5 && observeValue > rs.getDouble(3)){
                            resultMap.put("评价结果","劣V 类水质");
                            break;
                        }
                        resultMap.put("评价结果","");
                    }
                    list.add(resultMap);
                }
            }
        }catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(rs1 != null){
                rs1.close();
            }
            if(rs2 != null){
                rs2.close();
            }
            if(ps != null){
                ps.close();
            }
            if(ps1 != null){
                ps1.close();
            }
            if(ps2 != null){
                ps2.close();
            }
            if(call != null){
                call.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return list;
    }

    @Override
    public List<Map<Object, Object>> getPollutionItems(String objId, String water_body_id, String searchType, String period_id) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;

        PreparedStatement ps = null;
        PreparedStatement ps1 = null;

        ResultSet rs = null;
        ResultSet rs1 = null;

        //起止日期
        String startDate = null;
        String endDate = null;

        //存放的是最后的结果
        List resultList = new ArrayList();
        //存放污染物的项
        List<Map<Object,Object>> list = new ArrayList<Map<Object,Object>>();

        try {
            connection = basicDataSource.getConnection();
            Map<String,String> dateMap = getStartAndEndDate(period_id,searchType,connection);
            startDate = dateMap.get("startDate");
            endDate = dateMap.get("endDate");
            List<String> dmId = new ArrayList<String>();
            dmId.add(objId);
            //存放断面的指标项的指标id，指标值，指标名称
            List<Map<String,Object>> itemList = getDmOrSzItemList(connection,dmId,startDate,endDate);
            //查询断面或者水质站的代码，名称，水质等级
            String sql = "select t1.wr_sd_code, t2.rb_nm\n" +
                    "  from mgmb_河道断面信息简表 t1, wr_rb_b t2\n" +
                    " where t1.wr_sd_code = t2.rb_cd\n" +
                    "   and t1.河道断面_id = ?\n" +
                    "union\n" +
                    "select t3.wr_sd_code, t4.st_nm\n" +
                    "  from ZDST_水文测站信息简表 t3, WR_STAT_B t4\n" +
                    " where t3.wr_sd_code = t4.stcd\n" +
                    "   and t4.st_tp = 'WQ'\n" +
                    "   and t3.水文测站_id = ?\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,objId);
            ps.setString(2,objId);
            rs = ps.executeQuery();
            Map<Object,Object> dmOrSzMap = new HashMap<Object,Object>();
            if(rs.next()){
                dmOrSzMap.put("dmCode",rs.getString(1));
                dmOrSzMap.put("name",rs.getString(2));
            }
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            int dmOrSzLevel = getSZPJDmOrSz(connection,dmId,startDate,endDate);
            if(dmOrSzLevel > 0){
                String sql4 = "select 水质类别 from szpj_sys_断面水质类别定义 t where t.level_id = ?";
                ps = connection.prepareStatement(sql4);
                ps.setInt(1,dmOrSzLevel);
                rs = ps.executeQuery();
                if(rs.next()){
                    dmOrSzMap.put("waterGrade",rs.getString(1));
                }
                resultList.add(dmOrSzMap);
            }
            String sql1 = "select t1.item_id, t1.level_id, t1.value, t1.remark,t2.水质状况\n" +
                    "  from szpj_sys_指标达标值 t1, szpj_sys_断面水质类别定义 t2\n" +
                    " where t1.item_id = ?\n" +
                    "   and t1.level_id = t2.level_id\n" +
                    " order by t1.level_id";
            String sql2 = "select t.item_id, t.level_id, t.value, t.remark\n" +
                    "  from szpj_sys_指标达标值 t\n" +
                    " where t.item_id = ?\n" +
                    "   and t.level_id = 3\n";
            ps = connection.prepareStatement(sql1);
            ps1 = connection.prepareStatement(sql2);
            for(int i = 0 ; i < itemList.size() ; i++){
                Map<String,Object> map = itemList.get(i);
//                if("水温(℃)".equals(map.get("itemName")) || "溶解氧".equals(map.get("itemName")) || "PH值(无量纲)".equals(map.get("itemName"))){
//                    continue;
//                }
                Map<Object,Object> resultMap = new HashMap<Object,Object>();
                ps.setString(1, (String) map.get("itemId"));
                double observeValue = (double) map.get("itemAvgValue");
                resultMap.put("指标ID",map.get("itemId"));
                resultMap.put("测量值",(double)Math.round(observeValue*100)/100);
                resultMap.put("指标名称",map.get("itemName"));
                ps1.setString(1,(String) map.get("itemId"));
                rs1 = ps1.executeQuery();
                double excessValue = 0;
                if (rs1.next()){
                    //断面或者水质站的当前指标项的超标倍数
                    if(rs1.getString(3) != null){
                        double standardValue = rs1.getDouble(3);
                        excessValue = (observeValue-standardValue)/standardValue;
                        resultMap.put("超标情况",(double)Math.round(excessValue*100)/100);
                    }else{
                        resultMap.put("超标情况","");
                    }
                }else{
                    resultMap.put("超标情况","");
                }
                rs = ps.executeQuery();
                while (rs.next()){
                    if(">=".equals(rs.getString(4)) && observeValue >= rs.getDouble(3)){
                        resultMap.put("评价结果",rs.getString(5));
                        resultMap.put("level",rs.getInt(2));
                        break;
                    }
                    if(">=".equals(rs.getString(4)) && rs.getInt(2) == 5 && observeValue < rs.getDouble(3)){
                        resultMap.put("评价结果","劣V 类水质");
                        resultMap.put("level",6);
                        break;
                    }
                    if("<=".equals(rs.getString(4)) && observeValue <= rs.getDouble(3)){
                        resultMap.put("评价结果",rs.getString(5));
                        resultMap.put("level",rs.getInt(2));
                        break;
                    }
                    if("<=".equals(rs.getString(4)) && rs.getInt(2) == 5 && observeValue > rs.getDouble(3)){
                        resultMap.put("评价结果","劣V 类水质");
                        resultMap.put("level",6);
                        break;
                    }
                    resultMap.put("评价结果","");
                    resultMap.put("level","");
                }
                list.add(resultMap);
            }
            //对该断面或者水质站的指标数据进行整理
            //存放参与评选主要污染物的列表
            List<Map<Object,Object>> excessList = new ArrayList<Map<Object,Object>>();
            //存放结果的临时表
            List<Map<Object,Object>> tempList = new ArrayList<Map<Object,Object>>();
            for(int i = 0 ; i < list.size() ; i++){
                Map<Object,Object> map = list.get(i);
                if("".equals(map.get("level"))){
                    continue;
                }
                int num = (int) map.get("level");
                if(num > 3){
                    //存放参与评选主要污染物的列表
                    excessList.add(map);
                    if("氰化物".equals(map.get("指标名称")) || "铅".equals(map.get("指标名称")) || "铬(六价)".equals(map.get("指标名称"))){
                        Map<Object,Object> resultMap = new HashMap<Object,Object>();
                        resultMap.put("itemName",map.get("指标名称"));
                        resultMap.put("pollutionTimes",map.get("超标情况"));
                        resultMap.put("itemType",map.get("评价结果"));
                        resultMap.put("gcValue",map.get("测量值"));
                        //tempList存放重金属污染物列表
                        tempList.add(resultMap);
                    }
                }
            }
            MapSortUtil.desc(tempList,"pollutionTimes","double");
            //整理结果，降序排列
            if(tempList.size()<3){
                for(int i = 0 ; i < excessList.size() ; i++){
                    Map<Object,Object> map = excessList.get(i);
                    if("水温".equals(map.get("指标名称")) || "PH值(无量纲)".equals(map.get("指标名称")) || "溶解氧".equals(map.get("指标名称"))){
                        excessList.remove(map);
                    }
                }
                MapSortUtil.desc(excessList,"level","int");
                Set<Integer> levelSet = new HashSet<Integer>();
                for(int i = 0 ; i < excessList.size() ; i++){
                    Map<Object,Object> map = excessList.get(i);
                    levelSet.add((Integer) map.get("level"));
                }
                Integer[] levelArray = new Integer[levelSet.size()];
                levelSet.toArray(levelArray);
                //冒泡排序（降序）
                int temp;
                for (int i = 0; i < levelArray.length; i++) {
                    for (int j = i+1; j < levelArray.length; j++) {
                        if (levelArray[i] < levelArray[j]) {
                            temp = levelArray[i];
                            levelArray[i] = levelArray[j];
                            levelArray[j] = temp;	// 两个数交换位置
                        }
                    }
                }
                for(int i = 0 ; i < levelArray.length ; i++){
                    List<Map<Object,Object>> currentList = new ArrayList<Map<Object,Object>>();
                    for(int j = 0 ; j < excessList.size() ; j++){
                        Map<Object,Object> map = excessList.get(j);
                        if(levelArray[i] == map.get("level")){
                            Map<Object,Object> currentMap = new HashMap<Object,Object>();
                            currentMap.put("itemName",map.get("指标名称"));
                            currentMap.put("pollutionTimes",map.get("超标情况"));
                            currentMap.put("itemType",map.get("评价结果"));
                            currentMap.put("gcValue",map.get("测量值"));
                            currentList.add(currentMap);
                        }
                    }
                    MapSortUtil.desc(currentList,"pollutionTimes","double");
                    tempList.addAll(currentList);
                }
                if(tempList.size()>3){
                    resultList.add(tempList.subList(0,3));
                }else {
                    if(tempList.size()>0){
                        resultList.add(tempList);
                    }
                }
            }else{
                if(tempList.size()>0){
                    resultList.add(tempList);
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null){
                    rs.close();
                }
                if(rs1 != null){
                    rs1.close();
                }
                if(ps != null){
                    ps.close();
                }
                if(ps1 != null){
                    ps1.close();
                }
                if(call != null){
                    call.close();
                }
                if(connection != null){
                    connection.close();
                }
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }

        return resultList;
    }

    //这个方法是用来查询河流主要污染物指标的，暂时作废
    @Override
    public List<Map<Object,Object>> getSZPJItemValue(String water_body_id, String searchType, String period_id, String obj_id) throws SQLException{
        Connection connection = null;
        CallableStatement call = null;
        PreparedStatement ps = null;
        ResultSet rs = null;

        String startDate = null;
        String endDate = null;
        //最后返回结果
        List<Map<Object,Object>> resultList = new ArrayList<Map<Object,Object>>();

        try {
            connection = basicDataSource.getConnection();
            Map<String,String> dateMap = getStartAndEndDate(period_id,searchType,connection);
            startDate = dateMap.get("startDate");
            endDate = dateMap.get("endDate");
            connection.setAutoCommit(false);
            call = connection.prepareCall("{call SP_DOSZPJ(?,?,?,?,?,?,?,?)}");
            call.setString(1,water_body_id);
            call.setString(2,searchType);
            call.setString(3,obj_id);
            call.setString(4, startDate);
            call.setString(5, endDate);
            call.registerOutParameter(6, OracleTypes.CURSOR);
            call.registerOutParameter(7, OracleTypes.CURSOR);
            call.registerOutParameter(8, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(6);
            String levelID = null;
            String riverId = null;
            if(rs.next()){
                riverId = rs.getString("评价对象_ID");
                levelID = rs.getString("水质状况");
            }
            //河流水质不达标则查询河流的主要污染物指标
            if(!"优".equals(levelID) && !"良好".equals(levelID)){

            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                connection.commit();
                if(rs != null){
                    rs.close();
                }
                if(call != null){
                    call.close();
                }
                if(connection != null){
                    connection.close();
                }
            } catch (SQLException e) {
                connection.rollback();
                e.printStackTrace();
                throw e;
            }
        }
        return resultList;

    }

    @Override
    public List<Map<String, Object>> getWaterFounctionSZPJCount(String water_body_id, String riverID, String begin_year, String end_year) throws SQLException {
        //存放结果
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        //存放待整理数据
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        try {
            for(int i = Integer.parseInt(begin_year); i <= Integer.parseInt(end_year) ; i++){
                list.addAll(getSZPJWaterFounctionInRiver(riverID,i+"-01-01",i+"-12-31"));
            }

            Set<String> waterFounctionNames = new HashSet<String>();

            for(int i = 0 ; i < list.size() ; i++){
                String waterFounctionName = list.get(i).get("WR_SD_NM");
                waterFounctionNames.add(waterFounctionName);

            }
            Iterator waterFounctionName = waterFounctionNames.iterator();
            while (waterFounctionName.hasNext()){
                String currentWaterFounctionID = null;
                String currentWaterFounctionName = (String) waterFounctionName.next();
                String currentTotalWaterFounctionName = null;
                List<Map<String,String>> waterFounctionList = new ArrayList<Map<String, String>>();
                for(int j = 0 ; j < list.size() ; j++){
                    String name =  list.get(j).get("WR_SD_NM");
                    if(name.equals(currentWaterFounctionName)){
                        if(currentWaterFounctionID == null){
                            currentWaterFounctionID = list.get(j).get("评价对象_ID");
                            currentTotalWaterFounctionName = currentWaterFounctionName+"("+list.get(j).get("水质目标")+")";
                        }
                        waterFounctionList.add(list.get(j));
                    }
                }
                int dabiaoCount = 0;
                int countStep1 = 0;//I 类水质
                int countStep2 = 0;//II 类水质
                int countStep3 = 0;//III 类水质
                int countStep4 = 0;//IV 类水质
                int countStep5 = 0;//V 类水质
                int countStep6 = 0;//劣V 类水质

                for(int j = 0 ; j < waterFounctionList.size() ; j++){
                    if("0".equals(waterFounctionList.get(j).get("是否达标"))){
                        dabiaoCount++;
                    }
                    String waterFounctionCondition = (String) waterFounctionList.get(j).get("水质状况");
                    if("I 类水质".equals(waterFounctionCondition)){
                        countStep1++;
                    }
                    if("II 类水质".equals(waterFounctionCondition)){
                        countStep2++;
                    }
                    if("III 类水质".equals(waterFounctionCondition)){
                        countStep3++;
                    }
                    if("IV 类水质".equals(waterFounctionCondition)){
                        countStep4++;
                    }
                    if("V 类水质".equals(waterFounctionCondition)){
                        countStep5++;
                    }
                    if("劣V 类水质".equals(waterFounctionCondition)){
                        countStep6++;
                    }
                }
                int count = countStep1 + countStep2 + countStep3 + countStep4 + countStep5 + countStep6;
                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                DecimalFormat df  = new DecimalFormat("######0.00");
                resultMap.put("waterFounctionID",currentWaterFounctionID);
                resultMap.put("waterFounctionName",currentTotalWaterFounctionName);
                resultMap.put("次数总计",count);
                resultMap.put("I 类水质",count==0?"无数据":countStep1);
                resultMap.put("II 类水质",count==0?"无数据":countStep2);
                resultMap.put("III 类水质",count==0?"无数据":countStep3);
                resultMap.put("IV 类水质",count==0?"无数据":countStep4);
                resultMap.put("V 类水质",count==0?"无数据":countStep5);
                resultMap.put("劣V 类水质",count==0?"无数据":countStep6);
                resultMap.put("达标次数",count==0?"无数据":dabiaoCount);
                resultMap.put("达标百分比",count==0?"无数据":df.format(dabiaoCount/Double.parseDouble(count+"")*100)+"%");
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        return resultList;
    }

    @Override
    public Map<String,Object> getWaterFounctionSZPJCountDetail(String water_body_id, String riverId, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        Map<String,Object> endMap = new LinkedHashMap<String,Object>();
        //存放待整理数据
        try {
            List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
            List<Map<String,Object>> list = getWaterFounctionSZPJCount(water_body_id,riverId,begin_year,end_year);
            connection = basicDataSource.getConnection();
            String sql1 = "select t3.等级名称\n" +
                    "  from v_水功能区信息 t1, v_水资源对象 t2, mgmb_sys_水功能区分级表 t3\n" +
                    " where t1.水功能区_id = t2.obj_id\n" +
                    "   and t1.分级id = t3.sn_id\n" +
                    "   and t1.水功能区_id = ?\n";
            String sql3 = " SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                    "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                    "               ZDWARNER.WR_RB_B               RB,\n" +
                    "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                    "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                    "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                    "           and s.水功能区_id = ?";
            Set<String> set = new HashSet<String>();

            for(int i = 0 ; i < list.size() ; i++) {
                Map<String, Object> map = list.get(i);
                Map<String, Object> resultMap = new LinkedHashMap<String, Object>();
                resultMap.put("waterFounctionID", map.get("waterFounctionID"));
                resultMap.put("水功能区名称", map.get("waterFounctionName"));
                resultMap.put("达标次数", map.get("达标次数"));

                ps = connection.prepareStatement(sql1);
                ps.setString(1, (String) map.get("waterFounctionID"));
                rs = ps.executeQuery();
                String waterFounctionGrade = null;
                if(rs.next()){
                    waterFounctionGrade = rs.getString(1);
                }
                List<String> dmIdList = getDmOrSzIDList(connection, (String) map.get("waterFounctionID"));

                //存放该水功能区内所有断面的指标项
                for (int k = Integer.parseInt(begin_year); k <= Integer.parseInt(end_year); k++) {
                    List<Map<String,Object>> itemList = null;
                    if(dmIdList.size()==1){
                        itemList = getDmOrSzItemList(connection,dmIdList,k+"-01-01",k+"-12-31");
                    }else if(dmIdList.size() > 1){
                        if("缓冲区".equals(waterFounctionGrade)){
                            ps = connection.prepareStatement(sql3);
                            ps.setString(1,(String) map.get("waterFounctionID"));
                            rs = ps.executeQuery();
                            if (rs.next()){
                                List<String> currentList = new ArrayList<String>();
                                currentList.add(rs.getString(2));
                                itemList = getDmOrSzItemList(connection,currentList,k+"-01-01",k+"-12-31");
                            }
                        }else if("饮用水源区".equals(waterFounctionGrade)){
                            int level = 0;
                            //存放断面或者水质站的水质状况
                            Map<String,Object> waterCondition = new HashMap<String,Object>();
                            for(int m = 0 ; m < dmIdList.size() ; m++){
                                List<String> currentList = new ArrayList<String>();
                                currentList.add(dmIdList.get(m));
                                int currentLevel = getSZPJDmOrSz(connection,currentList,k+"-01-01",k+"-12-31");
                                waterCondition.put(dmIdList.get(m),currentLevel);
                                if(level < currentLevel){
                                    level = currentLevel;
                                }
                            }
                            Set<String> idSet = waterCondition.keySet();
                            Iterator id = idSet.iterator();
                            List<String> worstIdList = new ArrayList<String>();
                            while (id.hasNext()){
                                String currentId = (String) id.next();
                                if(waterCondition.get(currentId) == level){
                                    worstIdList.add(currentId);
                                }
                            }
                            itemList = getDmOrSzItemList(connection,worstIdList,k+"-01-01",k+"-12-31");
                        }else{
                            itemList = getDmOrSzItemList(connection,dmIdList,k+"-01-01",k+"-12-31");
                        }
                    }

                    Set<String> itemIdSet = new HashSet<String>();
                    for(int m = 0 ; m < itemList.size() ; m++){
                        itemIdSet.add((String) itemList.get(m).get("itemId"));
                    }
                    Iterator itemId = itemIdSet.iterator();
                    String sql2 = "select t2.sys_name, t1.value\n" +
                            "  from szpj_sys_指标达标值 t1, szpj_sys_指标项 t2\n" +
                            " where t1.item_id = t2.item_id\n" +
                            "   and t1.level_id = '3'\n" +
                            "   and t1.item_id = ?";

                    while (itemId.hasNext()) {
                        int itemCount = 0;
                        double standardItemValue = 0;
                        String currentItemId = (String) itemId.next();
                        String currentItemName = null;
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1, currentItemId);
                        rs = ps.executeQuery();
                        if (rs.next()) {
                            standardItemValue = rs.getDouble(2);
                        }
                        for (int n = 0; n < itemList.size(); n++) {
                            Map itemMap = itemList.get(n);
                            if (currentItemId.equals(itemMap.get("itemId"))) {
                                currentItemName = (String) itemMap.get("itemName");
                                double observeValue = (double) itemMap.get("itemAvgValue");
                                if (observeValue > standardItemValue) {
                                    itemCount++;
                                }
                            }
                        }
                        set.add(currentItemName);
                        resultMap.put(currentItemName, itemCount);
                    }
                }
                resultList.add(resultMap);
            }
            //存放数据的key，即表头
//            List<String> keyList = new ArrayList<String>();
//            keyList.add("waterFounctionID");
//            keyList.add("水功能区名称");
//            keyList.add("达标次数");
//            Iterator key = set.iterator();
//            while (key.hasNext()){
//                String currentKey = (String) key.next();
//                keyList.add(currentKey);
//            }

            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> map = resultList.get(i);
                Set<String> keySet = map.keySet();
                Set<String> tempSet = new HashSet<String>();
                tempSet.addAll(set);
                tempSet.removeAll(keySet);
                Iterator key = tempSet.iterator();
                while (key.hasNext()){
                    String currentKey = (String) key.next();
                    map.put(currentKey,"无数据");
                }
            }
            if(resultList.size()>0){
                Map<String,Object> map1 = resultList.get(0);
                List<String> keyList = new ArrayList<String>();
                for (Map.Entry<String, Object> entry : map1.entrySet()) {
                    keyList.add(entry.getKey());
                }
                endMap.put("titleList",keyList);
                endMap.put("data",resultList);
            }


//            for(int j = 0 ; j < resultList.size() ; j++){
//                Map<String,Object> map = resultList.get(j);
//                List<Map<String,Object>> currentItemList= new ArrayList<Map<String,Object>>();
//                Iterator itemName = set.iterator();
//                while (itemName.hasNext()){
//                    Map<String,Object> currentItemMap = new LinkedHashMap<String,Object>();
//                    String currentItemName = (String) itemName.next();
//                    currentItemMap.put("itemName",currentItemName);
//                    currentItemMap.put("itemCount",map.get(currentItemName));
//                    map.remove(currentItemName);
//                    currentItemList.add(currentItemMap);
//                }
//                map.put("itemInfo",currentItemList);
//            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return endMap;
    }

    @Override
    public List<Map<String, String>> getLegendByType(String type) {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> legendList = new ArrayList<Map<String,String>>();
        try {
            connection = basicDataSource.getConnection();
            if("wq".equals(type)){
                //水质评价图例
                String sql = "select 水质类别,color_value from szpj_sys_断面水质类别定义";
                ps = connection.prepareStatement(sql);
                rs = ps.executeQuery();
                while (rs.next()){
                    Map<String,String> legendMap = new LinkedHashMap<String,String>();
                    legendMap.put("type",rs.getString(1));
                    legendMap.put("color",rs.getString(2));
                    legendList.add(legendMap);
                }
            }else if("wr".equals(type)){
                //水资源承载力图例
                Map<String,String> legendMap1 = new LinkedHashMap<String,String>();
                legendMap1.put("type","不超载");
                legendMap1.put("color","#0x0000FF");
                legendList.add(legendMap1);

                Map<String,String> legendMap2 = new LinkedHashMap<String,String>();
                legendMap2.put("type","临界超载");
                legendMap2.put("color","#FCD209");
                legendList.add(legendMap2);

                Map<String,String> legendMap3 = new LinkedHashMap<String,String>();
                legendMap3.put("type","超载区");
                legendMap3.put("color","#FF0000");
                legendList.add(legendMap3);

            }else if("ns".equals(type)){
                //营养状态评价图例
                String sql = "select 等级名称,等级颜色 from szpj_sys_湖泊营养状态表";
                ps = connection.prepareStatement(sql);
                rs = ps.executeQuery();
                while (rs.next()){
                    Map<String,String> legendMap = new LinkedHashMap<String,String>();
                    legendMap.put("type",rs.getString(1));
                    legendMap.put("color",rs.getString(2));
                    legendList.add(legendMap);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return legendList;
    }

    public List<String> getDmOrSzIDList(Connection connection , String waterFounctionID) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<String> dmOrSzIdList = new ArrayList<String>();
        String sql1 = "select *\n" +
                "  from (SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.WR_WFZ_B              WFZ,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG,\n" +
                "               ZDWARNER.MGMB_水功能区信息简表 S\n" +
                "         WHERE WFZ.WFZ_STAR = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "           AND WFZ.WFZ_CD = S.WR_SD_CODE\n" +
                "        UNION\n" +
                "        SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.WR_WFZ_B              WFZ,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG,\n" +
                "               ZDWARNER.MGMB_水功能区信息简表 S\n" +
                "         WHERE WFZ.WFZ_END = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "           AND WFZ.WFZ_CD = S.WR_SD_CODE\n" +
                "        UNION\n" +
                "        SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                "         WHERE S.控制断面_CD = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "        UNION\n" +
                "        SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "        UNION\n" +
                "        SELECT T.水功能区_ID, T.河道断面_ID\n" +
                "          FROM ZDWARNER.MGMB_REL_断面与水体水功能区 T\n" +
                "         WHERE T.水功能区_ID IS NOT NULL\n" +
                "        UNION\n" +
                "        SELECT REL.水功能区_ID, REL.水文测站_ID 河道断面_ID\n" +
                "          FROM ZDWARNER.Mgmb_Rel_水功能区与水文测站 REL,\n" +
                "               ZDWARNER.V_水文测站信息              V\n" +
                "         WHERE REL.水文测站_ID = V.水文测站_ID) t\n" +
                " where t.水功能区_id = ?\n";
        try {
            ps = connection.prepareStatement(sql1);
            ps.setString(1,waterFounctionID);
            rs = ps.executeQuery();
            while (rs.next()){
                dmOrSzIdList.add(rs.getString(2));
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return dmOrSzIdList;
    }


    @Override
    public List<Map<String, Object>> getDMSZPJCount(String water_body_id, String waterFounctionID, String begin_year, String end_year) throws SQLException {
        Connection connection  = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            List<String> dmOrSzIdList = getDmOrSzIDList(connection,waterFounctionID);
            String sql2 = "select t2.rb_nm\n" +
                    "  from mgmb_河道断面信息简表 t1, wr_rb_b t2\n" +
                    " where t1.wr_sd_code = t2.rb_cd\n" +
                    "   and t1.河道断面_id = ?\n";

            if(rs != null){
                rs.close();
            }
            if(ps != null){
                rs.close();
            }
            for(int i = 0 ; i < dmOrSzIdList.size() ; i++){
                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                String currentId = dmOrSzIdList.get(i);
                String currentName = null;
                ps = connection.prepareStatement(sql2);
                ps.setString(1,currentId);
                rs = ps.executeQuery();
                if(rs.next()){
                    currentName = rs.getString(1);
                }
                List<String> current = new ArrayList<String>();
                current.add(currentId);
                int count = 0;
                int countStep1 = 0;//I 类水质
                int countStep2 = 0;//II 类水质
                int countStep3 = 0;//III 类水质
                int countStep4 = 0;//IV 类水质
                int countStep5 = 0;//V 类水质
                int countStep6 = 0;//劣V 类水质
                for(int j = Integer.valueOf(begin_year) ; j <= Integer.valueOf(end_year) ; j++){
                    int level = getSZPJDmOrSz(connection,current,j+"-01-01",j+"-12-31");
                    if(level == 1){
                        countStep1++;
                    }
                    if(level == 2){
                        countStep2++;
                    }
                    if(level == 3){
                        countStep3++;
                    }
                    if(level == 4){
                        countStep4++;
                    }
                    if(level == 5){
                        countStep5++;
                    }
                    if(level == 6){
                        countStep6++;
                    }
                }
                count = countStep1 + countStep2 + countStep3 + countStep4 + countStep5 + countStep6;
                resultMap.put("id",currentId);
                resultMap.put("name",currentName);
                resultMap.put("count",count);
                resultMap.put("I 类水质",count==0?"无数据":countStep1);
                resultMap.put("II 类水质",count==0?"无数据":countStep2);
                resultMap.put("III 类水质",count==0?"无数据":countStep3);
                resultMap.put("IV 类水质",count==0?"无数据":countStep4);
                resultMap.put("V 类水质",count==0?"无数据":countStep5);
                resultMap.put("劣V 类水质",count==0?"无数据":countStep6);
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps!= null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return resultList;
    }

    @Override
    public Map<String, Object> getDMSZPJCountDetail(String water_body_id, String waterFounctionID, String begin_year, String end_year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        Map<String,Object> endMap = new LinkedHashMap<String,Object>();
        //存放待整理数据
        try {
            List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
            List<Map<String,Object>> list = getDMSZPJCount(water_body_id,waterFounctionID,begin_year,end_year);
            connection = basicDataSource.getConnection();
            Set<String> set = new HashSet<String>();
            for(int i = 0 ; i < list.size() ; i++) {
                Map<String, Object> map = list.get(i);
                Map<String, Object> resultMap = new LinkedHashMap<String, Object>();
                resultMap.put("id", map.get("id"));
                resultMap.put("断面名称", map.get("name"));
                //存放该断面或者水质站的指标项
                List<String> currentId = new ArrayList<String>();
                currentId.add((String) map.get("id"));
                for (int k = Integer.parseInt(begin_year); k <= Integer.parseInt(end_year); k++) {
                    List<Map<String,Object>> itemList = getDmOrSzItemList(connection,currentId,k+"-01-01",k+"-12-31");
                    Set<String> itemIdSet = new HashSet<String>();
                    for(int m = 0 ; m < itemList.size() ; m++){
                        itemIdSet.add((String) itemList.get(m).get("itemId"));
                    }
                    Iterator itemId = itemIdSet.iterator();
                    String sql2 = "select t2.sys_name, t1.value\n" +
                            "  from szpj_sys_指标达标值 t1, szpj_sys_指标项 t2\n" +
                            " where t1.item_id = t2.item_id\n" +
                            "   and t1.level_id = '3'\n" +
                            "   and t1.item_id = ?";

                    while (itemId.hasNext()) {
                        int itemCount = 0;
                        double standardItemValue = 0;
                        String currentItemId = (String) itemId.next();
                        String currentItemName = null;
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1, currentItemId);
                        rs = ps.executeQuery();
                        if (rs.next()) {
                            standardItemValue = rs.getDouble(2);
                        }
                        for (int n = 0; n < itemList.size(); n++) {
                            Map itemMap = itemList.get(n);
                            if (currentItemId.equals(itemMap.get("itemId"))) {
                                currentItemName = (String) itemMap.get("itemName");
                                double observeValue = (double) itemMap.get("itemAvgValue");
                                if (observeValue > standardItemValue) {
                                    itemCount++;
                                }
                            }
                        }
                        set.add(currentItemName);
                        resultMap.put(currentItemName, itemCount);
                    }
                }
                resultList.add(resultMap);
            }

            for(int i = 0 ; i < resultList.size() ; i++){
                Map map = resultList.get(i);
                Set<String> keySet = map.keySet();
                Set<String> tempSet = new HashSet<String>();
                tempSet.addAll(set);
                tempSet.removeAll(keySet);
                Iterator key = tempSet.iterator();
                while (key.hasNext()){
                    String currentKey = (String) key.next();
                    map.put(currentKey,"无数据");
                }
            }
            if(resultList.size()>0){
                Map<String,Object> map1 = resultList.get(0);
                List<String> keyList = new ArrayList<String>();
                for (Map.Entry<String, Object> entry : map1.entrySet()) {
                    keyList.add(entry.getKey());
                }
                endMap.put("titleList",keyList);
                endMap.put("data",resultList);
            }


//            for(int j = 0 ; j < resultList.size() ; j++){
//                Map<String,Object> map = resultList.get(j);
//                List<Map<String,Object>> currentItemList= new ArrayList<Map<String,Object>>();
//                Iterator itemName = set.iterator();
//                while (itemName.hasNext()){
//                    Map<String,Object> currentItemMap = new LinkedHashMap<String,Object>();
//                    String currentItemName = (String) itemName.next();
//                    currentItemMap.put("itemName",currentItemName);
//                    currentItemMap.put("itemCount",map.get(currentItemName));
//                    map.remove(currentItemName);
//                    currentItemList.add(currentItemMap);
//                }
//                map.put("itemInfo",currentItemList);
//            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return endMap;
    }
//----------------------------水资源承载力------------------------------------
//获取某水功能区的水质监测值（化学需氧量、氨氮）(区分日期，求得每个日期的观测值)
    public List<Map<String,Object>> getSZYObserveValueByDate(Connection connection,String waterFounctionId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List<Map<String,Object>> resultList = null;
        try {
            String sql1 = "select t3.等级名称\n" +
                    "  from v_水功能区信息 t1, v_水资源对象 t2, mgmb_sys_水功能区分级表 t3\n" +
                    " where t1.水功能区_id = t2.obj_id\n" +
                    "   and t1.分级id = t3.sn_id\n" +
                    "   and t1.水功能区_id = ?\n";
            String sql2 = " SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                    "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                    "               ZDWARNER.WR_RB_B               RB,\n" +
                    "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                    "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                    "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                    "           and s.水功能区_id = ?";

            ps = connection.prepareStatement(sql1);
            ps.setString(1, waterFounctionId);
            rs = ps.executeQuery();
            String waterFounctionGrade = null;
            if(rs.next()){
                waterFounctionGrade = rs.getString(1);
            }
            List<String> dmIdList = getDmOrSzIDList(connection,waterFounctionId);
            if(dmIdList.size()==1){
                resultList = getDmOrSzTrueItemList(connection,dmIdList,startDate,endDate);
            }else if(dmIdList.size() > 1){
                if("缓冲区".equals(waterFounctionGrade)){
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1,waterFounctionId);
                    rs = ps.executeQuery();
                    if (rs.next()){
                        List<String> currentList = new ArrayList<String>();
                        currentList.add(rs.getString(2));
                        resultList = getDmOrSzTrueItemList(connection,currentList,startDate,endDate);
                    }
                }else if("饮用水源区".equals(waterFounctionGrade)){
                    int level = 0;
                    //存放断面或者水质站的水质状况
                    Map<String,Object> waterCondition = new HashMap<String,Object>();
                    for(int m = 0 ; m < dmIdList.size() ; m++){
                        List<String> currentList = new ArrayList<String>();
                        currentList.add(dmIdList.get(m));
                        int currentLevel = getSZPJDmOrSz(connection,currentList,startDate,endDate);
                        waterCondition.put(dmIdList.get(m),currentLevel);
                        if(level < currentLevel){
                            level = currentLevel;
                        }
                    }
                    Set<String> idSet = waterCondition.keySet();
                    Iterator id = idSet.iterator();
                    List<String> worstIdList = new ArrayList<String>();
                    while (id.hasNext()){
                        String currentId = (String) id.next();
                        if(waterCondition.get(currentId) == level){
                            worstIdList.add(currentId);
                        }
                    }
                    resultList = getDmOrSzTrueItemList(connection,worstIdList,startDate,endDate);
                }else{
                    resultList = getDmOrSzTrueItemList(connection,dmIdList,startDate,endDate);
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return resultList;
    }


    //获取某水功能区的水质监测值（化学需氧量、氨氮）(不区分日期，都是平均的)
    public List<Map<String,Object>> getSZYObserveValue(Connection connection,String waterFounctionId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List<Map<String,Object>> resultList = new ArrayList<Map<String,Object>>();
        try {
            String sql1 = "select t3.等级名称\n" +
                    "  from v_水功能区信息 t1, v_水资源对象 t2, mgmb_sys_水功能区分级表 t3\n" +
                    " where t1.水功能区_id = t2.obj_id\n" +
                    "   and t1.分级id = t3.sn_id\n" +
                    "   and t1.水功能区_id = ?\n";
            String sql2 = " SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                    "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                    "               ZDWARNER.WR_RB_B               RB,\n" +
                    "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                    "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                    "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                    "           and s.水功能区_id = ?";

            ps = connection.prepareStatement(sql1);
            ps.setString(1, waterFounctionId);
            rs = ps.executeQuery();
            String waterFounctionGrade = null;
            if(rs.next()){
                waterFounctionGrade = rs.getString(1);
            }
            List<String> dmIdList = getDmOrSzIDList(connection,waterFounctionId);
            List<Map<String,Object>> tempList = null;
            if(dmIdList.size()==1){
                tempList = getDmOrSzItemList(connection,dmIdList,startDate,endDate);
            }else if(dmIdList.size() > 1){
                if("缓冲区".equals(waterFounctionGrade)){
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1,waterFounctionId);
                    rs = ps.executeQuery();
                    if (rs.next()){
                        List<String> currentList = new ArrayList<String>();
                        currentList.add(rs.getString(2));
                        tempList = getDmOrSzItemList(connection,currentList,startDate,endDate);
                    }
                }else if("饮用水源区".equals(waterFounctionGrade)){
                    int level = 0;
                    //存放断面或者水质站的水质状况
                    Map<String,Object> waterCondition = new HashMap<String,Object>();
                    for(int m = 0 ; m < dmIdList.size() ; m++){
                        List<String> currentList = new ArrayList<String>();
                        currentList.add(dmIdList.get(m));
                        int currentLevel = getSZPJDmOrSz(connection,currentList,startDate,endDate);
                        waterCondition.put(dmIdList.get(m),currentLevel);
                        if(level < currentLevel){
                            level = currentLevel;
                        }
                    }
                    Set<String> idSet = waterCondition.keySet();
                    Iterator id = idSet.iterator();
                    List<String> worstIdList = new ArrayList<String>();
                    while (id.hasNext()){
                        String currentId = (String) id.next();
                        if(waterCondition.get(currentId) == level){
                            worstIdList.add(currentId);
                        }
                    }
                    tempList = getDmOrSzItemList(connection,worstIdList,startDate,endDate);
                }else{
                    tempList = getDmOrSzItemList(connection,dmIdList,startDate,endDate);
                }
            }
            if(tempList.size()>0){
                for(int i = 0 ; i < tempList.size() ; i++){
                    Map<String,Object> resultMap = tempList.get(i);
                    if("氨氮(NH3-N)".equals(resultMap.get("itemName")) || "化学需氧量(COD)".equals(resultMap.get("itemName"))){
                        resultList.add(resultMap);
                    }
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return resultList;
    }

    //获取某水功能区中参与水质评价的断面和水质站
    public List<String> getDmSzIds(Connection connection,String waterFounctionId,String startDate,String endDate) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<String> resultList = new ArrayList<String>();
        String sql1 = "select t3.等级名称\n" +
                "  from v_水功能区信息 t1, v_水资源对象 t2, mgmb_sys_水功能区分级表 t3\n" +
                " where t1.水功能区_id = t2.obj_id\n" +
                "   and t1.分级id = t3.sn_id\n" +
                "   and t1.水功能区_id = ?\n";
        String sql2 = " SELECT S.水功能区_ID, MG.河道断面_ID\n" +
                "          FROM ZDWARNER.MGMB_水功能区信息简表 S,\n" +
                "               ZDWARNER.WR_RB_B               RB,\n" +
                "               ZDWARNER.MGMB_河道断面信息简表 MG\n" +
                "         WHERE S.省界控制断面_CD = RB.RB_CD\n" +
                "           AND RB.RB_CD = MG.WR_SD_CODE\n" +
                "           and s.水功能区_id = ?";
        try {
            ps = connection.prepareStatement(sql1);
            ps.setString(1, waterFounctionId);
            rs = ps.executeQuery();
            String waterFounctionGrade = null;
            if(rs.next()){
                waterFounctionGrade = rs.getString(1);
            }
            List<String> dmIdList = getDmOrSzIDList(connection,waterFounctionId);
            if(dmIdList.size()==1){
                resultList.addAll(dmIdList);
            }else if(dmIdList.size() > 1){
                if("缓冲区".equals(waterFounctionGrade)){
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1,waterFounctionId);
                    rs = ps.executeQuery();
                    if (rs.next()){
                        resultList.add(rs.getString(2));
                    }
                }else if("饮用水源区".equals(waterFounctionGrade)){
                    int level = 0;
                    //存放断面或者水质站的水质状况
                    Map<String,Object> waterCondition = new HashMap<String,Object>();
                    for(int m = 0 ; m < dmIdList.size() ; m++){
                        List<String> currentList = new ArrayList<String>();
                        currentList.add(dmIdList.get(m));
                        int currentLevel = getSZPJDmOrSz(connection,currentList,startDate,endDate);
                        waterCondition.put(dmIdList.get(m),currentLevel);
                        if(level < currentLevel){
                            level = currentLevel;
                        }
                    }
                    Set<String> idSet = waterCondition.keySet();
                    Iterator id = idSet.iterator();
                    while (id.hasNext()){
                        String currentId = (String) id.next();
                        if(waterCondition.get(currentId) == level){
                            resultList.add(currentId);
                        }
                    }
                }else{
                    resultList.addAll(dmIdList);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null){
                ps.close();
            }
        }
        return resultList;
    }

    @Override
    public Map<String, Object> getSzyWaterQuality(List<Map<Object,Object>> xzqList,String water_body_id , String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;

        String startDate = year + "-01-01";
        String endDate = year + "-12-31";
        //存放所属行政区内的达标水功能区的个数达标率和长度达标率
        Map<String,Object> reachMap = new LinkedHashMap<String, Object>();

        //存放该行政区内的所有水功能区信息
        List<Map<String,Object>> waterFounctionList = new ArrayList<Map<String,Object>>();

        //开始进行水资源承载力之水功能区水质达标率
        try {
            //先通过行政区_ID找出对应的水功能区信息
            connection = basicDataSource.getConnection();
            Set<String> waterFounctionIdSet = new HashSet<String>();
            //通过xzqList里边的河道断面_ID也就是水功能区里边的控制断面找到当前年份的氨氮(NH3-N)和化学需氧量(COD)的相关信息
            for(int size = 0 ; size < xzqList.size() ; size++){
                List<Map<String,Object>> observeList = getSZYObserveValue(connection, (String) xzqList.get(size).get("水功能区_ID"),startDate,endDate);
                if(observeList.size() > 0){
                    for(int j = 0 ; j < observeList.size() ; j++){
                        Map<String,Object> waterFounctionMap = new LinkedHashMap<String,Object>();
                        waterFounctionIdSet.add((String) xzqList.get(size).get("水功能区_ID"));
                        waterFounctionMap.put("水功能区ID",xzqList.get(size).get("水功能区_ID"));
                        waterFounctionMap.put("水质目标",xzqList.get(size).get("水质目标"));
                        waterFounctionMap.put("行政区划名称", xzqList.get(size).get("行政区划名称"));
                        waterFounctionMap.put("水功能区经纬度",xzqList.get(size).get("经纬度坐标"));
                        waterFounctionMap.put("水功能区名称",xzqList.get(size).get("WFZ_NM"));
                        waterFounctionMap.putAll(observeList.get(j));
                        waterFounctionList.add(waterFounctionMap);
                    }
                }
            }

            if(waterFounctionList.size()>0) {
                //开始计算比较是否达标
                //存放达标的水功能区ID
                List<String> waterFunctionIDs = new ArrayList<String>();
                //行政区名称
                String name = null;
                //存放某一条河流某一个行政区内的所有水功能区信息
                List<Map<String,Object>> resultWaterFounctionList = new ArrayList<Map<String,Object>>();

                Iterator waterFounctionId = waterFounctionIdSet.iterator();
                while (waterFounctionId.hasNext()){
                    Map<String,Object> resultWaterFounctionMap = new LinkedHashMap<String,Object>();
                    String currentWaterFounctionId = (String) waterFounctionId.next();
                    String currentWaterFounctionName = null;
                    double CODValue = 0;
                    double NHValue = 0;
                    int CODCount = 0;
                    int NHCount = 0;
                    boolean CODFlag = false;
                    boolean NHFlag = false;
                    String waterTarget = null;
                    for(int i = 0 ; i < waterFounctionList.size() ; i++){
                        Map<String,Object> map = waterFounctionList.get(i);
                        if(currentWaterFounctionId.equals(map.get("水功能区ID"))){
                            if(waterTarget == null){
                                waterTarget = (String) map.get("水质目标");
                            }
                            if(currentWaterFounctionName == null){
                                currentWaterFounctionName = (String) map.get("水功能区名称");
                            }
                            if("化学需氧量(COD)".equals(map.get("itemName"))){
                                CODValue = (double) map.get("itemAvgValue");
                                CODCount = (int) map.get("observeCount");
                                CODFlag = true;
                            }
                            if("氨氮(NH3-N)".equals(map.get("itemName"))){
                                NHValue = (double) map.get("itemAvgValue");
                                NHCount = (int) map.get("observeCount");
                                NHFlag = true;
                            }
                        }
                    }
                    resultWaterFounctionMap.put("funCode",currentWaterFounctionId);
                    resultWaterFounctionMap.put("funName",currentWaterFounctionName);

                    int maxCount = 0;
                    if(CODCount <= NHCount){
                        maxCount = NHCount;
                    }else{
                        maxCount = CODCount;
                    }
                    //根据监测次数的不同来进行不同的达标的计算
                    //监测次数大于等于6次，平均观测值和III类水质比较

                    //先获取参与水功能区水质评价的断面和水质站
                    List<String> idList = getDmSzIds(connection,currentWaterFounctionId,startDate,endDate);
                    //二、获取断面、水质站的观测值和观测日期
                    List<Map<String,Object>> itemTrueList = getDmOrSzTrueItemList(connection,idList,startDate,endDate);

                    Set<String> dateSet = new HashSet<String>();
                    for(int i = 0 ; i < itemTrueList.size() ; i++){
                        Map<String,Object> map = itemTrueList.get(i);
                        dateSet.add((String) map.get("observeDate"));
                    }

                    //达标次数
                    int countDB = 0;

                    if(maxCount >= 6){
                        if(CODFlag && NHFlag){
                            if(CODValue <= 20 && NHValue <= 1){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        if(CODFlag && !NHFlag){
                            if(CODValue <= 20){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        if(!CODFlag && NHFlag){
                            if(NHValue <= 1){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        Iterator date = dateSet.iterator();
                        while (date.hasNext()){
                            String currentDate = (String) date.next();
                            for(int j = 0 ; j < itemTrueList.size() ; j++){
                                Map<String,Object> map = itemTrueList.get(j);
                                if(currentDate.equals(map.get("observeDate"))){
                                    if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                        if(((double)map.get("化学需氧量(COD)"))<=20 && ((double)map.get("氨氮(NH3-N)")) <= 1){
                                            countDB++;
                                        }
                                    }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                        if(((double)map.get("氨氮(NH3-N)")) <= 1){
                                            countDB++;
                                        }
                                    }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                        if(((double)map.get("化学需氧量(COD)")) <= 20){
                                            countDB++;
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if("1".equals(waterTarget)){
                            //监测次数小于6次，水质指标是I类的，求平均观测值和I类水质比较
                            if(CODFlag && NHFlag){
                                if(CODValue <= 15 && NHValue <= 0.15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            if(CODFlag && !NHFlag){
                                if(CODValue <= 15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            if(!CODFlag && NHFlag){
                                if(NHValue <= 0.15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            Iterator date = dateSet.iterator();
                            while (date.hasNext()){
                                String currentDate = (String) date.next();
                                for(int j = 0 ; j < itemTrueList.size() ; j++){
                                    Map<String,Object> map = itemTrueList.get(j);
                                    if(currentDate.equals(map.get("observeDate"))){
                                        if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("化学需氧量(COD)"))<=15 && ((double)map.get("氨氮(NH3-N)")) <= 0.15){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("氨氮(NH3-N)")) <= 0.15){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                            if(((double)map.get("化学需氧量(COD)")) <= 15){
                                                countDB++;
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            //三、通过观测日期，比较是否达标(和III类水质比较)，达标次数在80%以上 ，该水功能区才能达标
                            Iterator date = dateSet.iterator();
                            while (date.hasNext()){
                                String currentDate = (String) date.next();
                                for(int j = 0 ; j < itemTrueList.size() ; j++){
                                    Map<String,Object> map = itemTrueList.get(j);
                                    if(currentDate.equals(map.get("observeDate"))){
                                        if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("化学需氧量(COD)"))<=20 && ((double)map.get("氨氮(NH3-N)")) <= 1){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("氨氮(NH3-N)")) <= 1){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                            if(((double)map.get("化学需氧量(COD)")) <= 20){
                                                countDB++;
                                            }
                                        }
                                    }
                                }
                            }
                            if(dateSet.size() > 0 && countDB/dateSet.size()>=0.8){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }

                        }
                    }


                    DecimalFormat df  = new DecimalFormat("######0.00");
                    resultWaterFounctionMap.put("attainedPercent",df.format(countDB/maxCount*100)+"%");
                    resultWaterFounctionList.add(resultWaterFounctionMap);
                }

                double upLenth = 0;
                double totalUpLength = 0;
                String position = null;
                //该行政区的达标水功能区的上游长度
                for (int num = 0; num < waterFunctionIDs.size(); num++) {
                    for (int x = 0; x < xzqList.size(); x++) {
                        Map map3 = (Map) xzqList.get(x);
                        if (waterFunctionIDs.get(num).equals(map3.get("水功能区_ID"))) {
                            BigDecimal length = (BigDecimal) map3.get("上游长度");
                            upLenth = upLenth + length.doubleValue();
                        }
                    }
                }

                for(int i = 0 ; i < waterFounctionList.size() ; i++){
                        Map<String,Object> map = waterFounctionList.get(i);
                        if(position == null){
                            position = (String) map.get("水功能区经纬度");
                        }
                }
                //该行政区的水功能区上游总长(有观测值的水功能区)
                for (int x = 0; x < xzqList.size(); x++) {
                    Map map3 = (Map) xzqList.get(x);
                    BigDecimal length2 = (BigDecimal) map3.get("上游长度");
                    totalUpLength = totalUpLength + length2.doubleValue();
                }

                DecimalFormat df  = new DecimalFormat("######0.00");
                double size1 = waterFunctionIDs.size();
                double size2 = xzqList.size();//totalDmList.size();
                double size = size1/size2*100;

    //                reachMap.put("行政区划名称", name);
                reachMap.put("totalCount",size2);
                reachMap.put("attainedCount",size1);

                reachMap.put("个数达标率", df.format(size)  + "%");
                reachMap.put("attainedPercent", df.format(size)  + "%");
                reachMap.put("totalLength",totalUpLength);
                reachMap.put("attainedLength",upLenth);

                reachMap.put("长度达标率", df.format(upLenth / totalUpLength * 100) + "%");
                reachMap.put("lengthPercent", df.format(upLenth / totalUpLength * 100) + "%");
                reachMap.put("x",position.split(",")[0]);
                reachMap.put("y",position.split(",")[1]);
                reachMap.put("水功能区详细信息",resultWaterFounctionList);
    //                list.add(reachMap);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null){
                    rs.close();
                }
                if(ps != null){
                    ps.close();
                }
                if(connection != null){
                    connection.close();
                }
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
        return reachMap;
    }

    @Override
    public List<Map<Object, Object>> getRiverWaterFounctionCondition(String water_body_id, String xzq_id, String year, String riverID) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List riverWaterFounctionList = new ArrayList();
        try {
            connection = basicDataSource.getConnection();
            List list = getOneXZQCondition(riverID, xzq_id, year);
            for(int i = 0 ; i < list.size() ; i++){
                Map map = (Map) list.get(i);
                List list4 = (List) map.get("水功能区详细信息");
                for(int j = 0 ; j < list4.size() ; j++){
                    Map map1 = (Map) list4.get(j);
                    Map riverWaterFounctionMap = new LinkedHashMap();
                    riverWaterFounctionMap.put("code",map.get("code"));
                    riverWaterFounctionMap.put("name", map.get("name"));
                    riverWaterFounctionMap.put("funCode",map1.get("funCode"));
                    riverWaterFounctionMap.put("funName",(String)map.get("name")+map1.get("funName"));
                    riverWaterFounctionMap.put("isAttained",map1.get("isAttained"));
                    riverWaterFounctionMap.put("attainedPercent",map1.get("attainedPercent"));

                    String sql1 = "select t4.上游行政区_id,\n" +
                            "       t5.行政区划名称,\n" +
                            "       t1.监测日期,\n" +
                            "       t4.水功能区_id,\n" +
                            "       t6.item_id,\n" +
                            "       t6.sys_name,\n" +
                            "       t1.污染物入河量\n" +
                            "  from szpj_biz_排污口污染物入河量 t1,\n" +
                            "       mgmb_入河排污口信息简表     t2,\n" +
                            "       wr_pdo_b                    t3,\n" +
                            "       mgmb_水功能区信息简表       t4,\n" +
                            "       v_行政区划                  t5,\n" +
                            "       szpj_sys_指标项             t6,\n" +
                            "       mgmb_rel_水功能区与水体     t7\n" +
                            " where t1.排污口_id = t2.入河排污口_id\n" +
                            "   and to_number(to_char(t1.监测日期, 'yyyy')) between ? and ?\n" +
                            "   and t2.wr_sd_code = t3.pdo_cd\n" +
                            "   and t3.wfz_cd = t4.wr_sd_code\n" +
                            "   and t4.上游行政区_id = t5.行政区_id\n" +
                            "   and t4.上游行政区_id = ?\n" +
                            "   and t6.item_id = t1.污染物指标_id\n" +
                            "   and t7.水功能区_id = t4.水功能区_id\n" +
                            "   and t7.水体_id = ?\n";
                    ps = connection.prepareStatement(sql1);
                    ps.setInt(1, Integer.valueOf(year));
                    ps.setInt(2, Integer.valueOf(year));
                    ps.setString(3, xzq_id);
                    ps.setString(4,riverID);
                    rs = ps.executeQuery();
                    double valueNH = 0;
                    double valueCOD = 0;
                    while (rs.next()){
                        if(map1.get("funCode").equals(rs.getString("水功能区_ID"))){
                            if("氨氮(NH3-N)".equals(rs.getString("SYS_NAME"))){
                                valueNH = valueNH + rs.getDouble("污染物入河量");
                            }
                            if("化学需氧量(COD)".equals(rs.getString("SYS_NAME"))){
                                valueCOD = valueCOD + rs.getDouble("污染物入河量");
                            }
                        }
                    }
                    riverWaterFounctionMap.put("cod",valueCOD);
                    riverWaterFounctionMap.put("inputValue", valueNH);
                    rs.close();
                    ps.close();

                    riverWaterFounctionMap.put("monDetail", "");
                    String sql2 = "select 经纬度坐标 from mgmb_水功能区信息简表 where 水功能区_id = ?";
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1, (String) map1.get("funCode"));
                    rs = ps.executeQuery();
                    if(rs.next()){
                        String place = rs.getString(1);
                        riverWaterFounctionMap.put("x",place.split(",")[0]);
                        riverWaterFounctionMap.put("y",place.split(",")[1]);
                    }
                    riverWaterFounctionList.add(riverWaterFounctionMap);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs!=null){
                rs.close();
            }
            if(ps!=null){
                ps.close();
            }
            if(connection!=null){
                connection.close();
            }
        }
        return riverWaterFounctionList;
    }

    public Map<String, Object> getSzyWaterQualityT(List<Map<Object,Object>> xzqList,String water_body_id , String xzq_id, String year) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;

        String startDate = year + "-01-01";
        String endDate = year + "-12-31";
        //存放所属行政区内的达标水功能区的个数达标率和长度达标率
        Map<String,Object> reachMap = new LinkedHashMap<String, Object>();

        //存放该行政区内的所有水功能区信息
        List<Map<String,Object>> waterFounctionList = new ArrayList<Map<String,Object>>();

        //开始进行水资源承载力之水功能区水质达标率
        try {
            //先通过行政区_ID找出对应的水功能区信息
            connection = basicDataSource.getConnection();
            Set<String> waterFounctionIdSet = new HashSet<String>();
            //通过xzqList里边的河道断面_ID也就是水功能区里边的控制断面找到当前年份的氨氮(NH3-N)和化学需氧量(COD)的相关信息
            for(int size = 0 ; size < xzqList.size() ; size++){
                List<Map<String,Object>> observeList = getSZYObserveValue(connection, (String) xzqList.get(size).get("水功能区_ID"),startDate,endDate);
                if(observeList.size() > 0){
                    for(int j = 0 ; j < observeList.size() ; j++){
                        Map<String,Object> waterFounctionMap = new LinkedHashMap<String,Object>();
                        waterFounctionIdSet.add((String) xzqList.get(size).get("水功能区_ID"));
                        waterFounctionMap.put("水功能区ID",xzqList.get(size).get("水功能区_ID"));
                        waterFounctionMap.put("水质目标",xzqList.get(size).get("水质目标"));
                        waterFounctionMap.put("行政区划名称", xzqList.get(size).get("行政区划名称"));
                        waterFounctionMap.put("水功能区经纬度",xzqList.get(size).get("经纬度坐标"));
                        waterFounctionMap.put("水功能区名称",xzqList.get(size).get("WFZ_NM"));
                        waterFounctionMap.putAll(observeList.get(j));
                        waterFounctionList.add(waterFounctionMap);
                    }
                }
            }
            Set<String> funId = new HashSet<String>();
            Set<String> funId1 = new HashSet<String>();
            Set<String> funId2 = new HashSet<String>();
            for(int i = 0 ; i < waterFounctionList.size() ; i++){
                Map map = waterFounctionList.get(i);
                funId1.add((String)map.get("水功能区ID"));
            }
            for(int size = 0 ; size < xzqList.size() ; size++){
                Map map = xzqList.get(size);
                funId2.add((String) map.get("水功能区_ID"));
            }
            funId.addAll(funId2);
            funId.removeAll(funId1);
            //存放某一条河流某一个行政区内的所有水功能区信息
            List<Map<String,Object>> resultWaterFounctionList = new ArrayList<Map<String,Object>>();
            if(funId.size() > 0){
                Iterator ids = funId.iterator();
                while (ids.hasNext()){
                    String currentId = (String) ids.next();
                    for(int k = 0 ; k < xzqList.size() ; k++){
                        Map map = xzqList.get(k);
                        Map waterFunc = new LinkedHashMap();
                        if(currentId.equals( map.get("水功能区_ID"))){
                            waterFunc.put("code",map.get("水体_ID"));
                            waterFunc.put("name",map.get("水体名称"));
                            waterFunc.put("funCode",map.get("水功能区_ID"));
                            waterFunc.put("funName",map.get("WFZ_NM"));
                            waterFunc.put("isAttained","");
                            waterFunc.put("attainedPercent","0%");
                            waterFunc.put("x",((String)map.get("经纬度坐标")).split(",")[0]);
                            waterFunc.put("y",((String)map.get("经纬度坐标")).split(",")[1]);
                            resultWaterFounctionList.add(waterFunc);
                        }
                    }
                }
            }
            if(waterFounctionList.size()>0) {
                //开始计算比较是否达标
                //存放达标的水功能区ID
                List<String> waterFunctionIDs = new ArrayList<String>();
                //行政区名称
                String name = null;


                Iterator waterFounctionId = waterFounctionIdSet.iterator();
                while (waterFounctionId.hasNext()){
                    Map<String,Object> resultWaterFounctionMap = new LinkedHashMap<String,Object>();
                    String currentWaterFounctionId = (String) waterFounctionId.next();
                    String currentWaterFounctionName = null;
                    double CODValue = 0;
                    double NHValue = 0;
                    int CODCount = 0;
                    int NHCount = 0;
                    boolean CODFlag = false;
                    boolean NHFlag = false;
                    String waterTarget = null;
                    for(int i = 0 ; i < waterFounctionList.size() ; i++){
                        Map<String,Object> map = waterFounctionList.get(i);
                        if(currentWaterFounctionId.equals(map.get("水功能区ID"))){
                            if(waterTarget == null){
                                waterTarget = (String) map.get("水质目标");
                            }
                            if(currentWaterFounctionName == null){
                                currentWaterFounctionName = (String) map.get("水功能区名称");
                            }
                            if("化学需氧量(COD)".equals(map.get("itemName"))){
                                CODValue = (double) map.get("itemAvgValue");
                                CODCount = (int) map.get("observeCount");
                                CODFlag = true;
                            }
                            if("氨氮(NH3-N)".equals(map.get("itemName"))){
                                NHValue = (double) map.get("itemAvgValue");
                                NHCount = (int) map.get("observeCount");
                                NHFlag = true;
                            }
                        }
                    }
                    resultWaterFounctionMap.put("funCode",currentWaterFounctionId);
                    resultWaterFounctionMap.put("funName",currentWaterFounctionName);

                    int maxCount = 0;
                    if(CODCount <= NHCount){
                        maxCount = NHCount;
                    }else{
                        maxCount = CODCount;
                    }
                    //根据监测次数的不同来进行不同的达标的计算
                    //监测次数大于等于6次，平均观测值和III类水质比较

                    //先获取参与水功能区水质评价的断面和水质站
                    List<String> idList = getDmSzIds(connection,currentWaterFounctionId,startDate,endDate);
                    //二、获取断面、水质站的观测值和观测日期
                    List<Map<String,Object>> itemTrueList = getDmOrSzTrueItemList(connection,idList,startDate,endDate);

                    Set<String> dateSet = new HashSet<String>();
                    for(int i = 0 ; i < itemTrueList.size() ; i++){
                        Map<String,Object> map = itemTrueList.get(i);
                        dateSet.add((String) map.get("observeDate"));
                    }

                    //达标次数
                    int countDB = 0;

                    if(maxCount >= 6){
                        if(CODFlag && NHFlag){
                            if(CODValue <= 20 && NHValue <= 1){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        if(CODFlag && !NHFlag){
                            if(CODValue <= 20){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        if(!CODFlag && NHFlag){
                            if(NHValue <= 1){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }
                        }
                        Iterator date = dateSet.iterator();
                        while (date.hasNext()){
                            String currentDate = (String) date.next();
                            for(int j = 0 ; j < itemTrueList.size() ; j++){
                                Map<String,Object> map = itemTrueList.get(j);
                                if(currentDate.equals(map.get("observeDate"))){
                                    if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                        if(((double)map.get("化学需氧量(COD)"))<=20 && ((double)map.get("氨氮(NH3-N)")) <= 1){
                                            countDB++;
                                        }
                                    }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                        if(((double)map.get("氨氮(NH3-N)")) <= 1){
                                            countDB++;
                                        }
                                    }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                        if(((double)map.get("化学需氧量(COD)")) <= 20){
                                            countDB++;
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        if("1".equals(waterTarget)){
                            //监测次数小于6次，水质指标是I类的，求平均观测值和I类水质比较
                            if(CODFlag && NHFlag){
                                if(CODValue <= 15 && NHValue <= 0.15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            if(CODFlag && !NHFlag){
                                if(CODValue <= 15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            if(!CODFlag && NHFlag){
                                if(NHValue <= 0.15){
                                    waterFunctionIDs.add(currentWaterFounctionId);
                                    resultWaterFounctionMap.put("isAttained", "是");
                                }else{
                                    resultWaterFounctionMap.put("isAttained", "否");
                                }
                            }
                            Iterator date = dateSet.iterator();
                            while (date.hasNext()){
                                String currentDate = (String) date.next();
                                for(int j = 0 ; j < itemTrueList.size() ; j++){
                                    Map<String,Object> map = itemTrueList.get(j);
                                    if(currentDate.equals(map.get("observeDate"))){
                                        if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("化学需氧量(COD)"))<=15 && ((double)map.get("氨氮(NH3-N)")) <= 0.15){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("氨氮(NH3-N)")) <= 0.15){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                            if(((double)map.get("化学需氧量(COD)")) <= 15){
                                                countDB++;
                                            }
                                        }
                                    }
                                }
                            }
                        }else{
                            //三、通过观测日期，比较是否达标(和III类水质比较)，达标次数在80%以上 ，该水功能区才能达标
                            Iterator date = dateSet.iterator();
                            while (date.hasNext()){
                                String currentDate = (String) date.next();
                                for(int j = 0 ; j < itemTrueList.size() ; j++){
                                    Map<String,Object> map = itemTrueList.get(j);
                                    if(currentDate.equals(map.get("observeDate"))){
                                        if( map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("化学需氧量(COD)"))<=20 && ((double)map.get("氨氮(NH3-N)")) <= 1){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") == null && map.get("氨氮(NH3-N)") != null){
                                            if(((double)map.get("氨氮(NH3-N)")) <= 1){
                                                countDB++;
                                            }
                                        }else if(map.get("化学需氧量(COD)") != null && map.get("氨氮(NH3-N)") == null){
                                            if(((double)map.get("化学需氧量(COD)")) <= 20){
                                                countDB++;
                                            }
                                        }
                                    }
                                }
                            }
                            if(dateSet.size() > 0 && countDB/dateSet.size()>=0.8){
                                waterFunctionIDs.add(currentWaterFounctionId);
                                resultWaterFounctionMap.put("isAttained", "是");
                            }else{
                                resultWaterFounctionMap.put("isAttained", "否");
                            }

                        }
                    }


                    DecimalFormat df  = new DecimalFormat("######0.00");
                    resultWaterFounctionMap.put("attainedPercent",df.format(countDB/maxCount*100)+"%");
                    resultWaterFounctionList.add(resultWaterFounctionMap);
                }

                double upLenth = 0;
                double totalUpLength = 0;
                String position = null;
                //该行政区的达标水功能区的上游长度
                for (int num = 0; num < waterFunctionIDs.size(); num++) {
                    for (int x = 0; x < xzqList.size(); x++) {
                        Map map3 = (Map) xzqList.get(x);
                        if (waterFunctionIDs.get(num).equals(map3.get("水功能区_ID"))) {
                            BigDecimal length = (BigDecimal) map3.get("上游长度");
                            upLenth = upLenth + length.doubleValue();
                        }
                    }
                }

                for(int i = 0 ; i < waterFounctionList.size() ; i++){
                    Map<String,Object> map = waterFounctionList.get(i);
                    if(position == null){
                        position = (String) map.get("水功能区经纬度");
                    }
                }
                //该行政区的水功能区上游总长(有观测值的水功能区)
                for (int x = 0; x < xzqList.size(); x++) {
                    Map map3 = (Map) xzqList.get(x);
                    BigDecimal length2 = (BigDecimal) map3.get("上游长度");
                    totalUpLength = totalUpLength + length2.doubleValue();
                }

                DecimalFormat df  = new DecimalFormat("######0.00");
                double size1 = waterFunctionIDs.size();
                double size2 = xzqList.size();//totalDmList.size();
                double size = size1/size2*100;

                //                reachMap.put("行政区划名称", name);
                reachMap.put("totalCount",size2);
                reachMap.put("attainedCount",size1);

                reachMap.put("个数达标率", df.format(size)  + "%");
                reachMap.put("attainedPercent", df.format(size)  + "%");
                reachMap.put("totalLength",totalUpLength);
                reachMap.put("attainedLength",upLenth);

                reachMap.put("长度达标率", df.format(upLenth / totalUpLength * 100) + "%");
                reachMap.put("lengthPercent", df.format(upLenth / totalUpLength * 100) + "%");
                reachMap.put("x",position.split(",")[0]);
                reachMap.put("y",position.split(",")[1]);
                reachMap.put("水功能区详细信息",resultWaterFounctionList);
                //                list.add(reachMap);
            }else{
                reachMap.put("水功能区详细信息",resultWaterFounctionList);
            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            try {
                if(rs != null){
                    rs.close();
                }
                if(ps != null){
                    ps.close();
                }
                if(connection != null){
                    connection.close();
                }
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }
        return reachMap;
    }
}
