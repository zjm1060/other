package bitc.sz.wzEnv.szpj;

import bitc.sz.wzEnv.szpj.model.*;
import bitc.sz.wzEnv.util.JsonUtil;
import com.sun.xml.xsom.impl.scd.Iterators;
import oracle.jdbc.OracleTypes;
import org.codehaus.jackson.type.TypeReference;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.sql.DataSource;
import java.io.IOException;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.Date;

/**
 * Created by Administrator on 2016/3/9.
 */
@Service
public class SzpjServiceImpl implements SzpjService {
    @Autowired
    private DataSource basicDataSource;

    public DataSource getBasicDataSource() {
        return basicDataSource;
    }

    public void setBasicDataSource(DataSource basicDataSource) {
        this.basicDataSource = basicDataSource;
    }

    @Override
    public List<SzType> getSzType() throws SQLException {
        List<SzType> szTypes= new ArrayList<SzType>();
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "select unique DOMAIN_ID,DOMAIN_NAME from szpj_sys_指标项 order by DOMAIN_ID";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while(rs.next()){
                SzType szType = new SzType();
                szType.setDOMAIN_ID(rs.getString("DOMAIN_ID"));
                szType.setDOMAIN_NAME(rs.getString("DOMAIN_NAME"));
                szTypes.add(szType);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            rs.close();
            ps.close();
            connection.close();
        }
        return szTypes;
    }

    @Override
    public List<ZbType> getZbType(String DOMAIN_ID) throws Exception{
        List<ZbType> zbTypes = new ArrayList<ZbType>();
        Connection connection =  basicDataSource.getConnection();
        String sql = "select unique ITYPE_ID,TYPE_NAME from szpj_sys_指标项 where DOMAIN_ID = ?  ORDER BY itype_id";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, DOMAIN_ID);
        ResultSet rs = ps.executeQuery();
        while(rs.next()){
            ZbType zbType = new ZbType();
            zbType.setITYPE_ID(rs.getString("ITYPE_ID"));
            zbType.setTYPE_NAME(rs.getString("TYPE_NAME"));
            zbTypes.add(zbType);
        }
        rs.close();
        ps.close();
        connection.close();
        return zbTypes;
    }

    @Override
    public List<Places> getPlaces(String waterBodyId,String keywords) throws Exception{
        if("".equals(keywords)){
            keywords=null;
        }
        List<Places> places = new ArrayList<Places>();
        Connection connection = basicDataSource.getConnection();
            CallableStatement call = connection.prepareCall("{call SP_GETMONITOROBJ(?,?,?,?)}");
            call.setString(1, waterBodyId);
            call.setString(2,"断面");
            call.setString(3, keywords);
            call.registerOutParameter(4, OracleTypes.CURSOR);
            call.execute();
            ResultSet rs= (ResultSet)call.getObject(4);
            while (rs.next()){
                Places place = new Places();
                place.setObj_id(rs.getString(1));
                place.setType(rs.getString(2));
                place.setName(rs.getString(3));
                places.add(place);
            }
        rs.close();
        call.close();
        connection.close();
        return places;
    }

    @Override
    public List<Map<String, String>> getWaterBody() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String, String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t.obj_id,t.obj_name from v_用户对应水体 t";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("waterBodyId",rs.getString(1));
                resultMap.put("waterBodyName",rs.getString(2));
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
    public List<ZbValue> getZbValue(String DOMAIN_ID, String TYPEID, String MONITORID,String MONITORSIDE, String MONITORDATE) throws Exception{
        List<ZbValue> zbValues = new ArrayList<ZbValue>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date = null;

        if(MONITORDATE!=null&&(!"".equals(MONITORDATE))){
//            String beforeDate = DateUtil.getYearMonthDayHourMinuteSecond(MONITORDATE);
//            formatDate = Timestamp.valueOf(beforeDate);
//            java.util.Date date = DateUtil.parseDateTime(beforeDate);
            date = sdf.format(new Date(Long.valueOf(MONITORDATE)));
        }else{
            date = null;
        }
        Connection connection = basicDataSource.getConnection();
        CallableStatement call = connection.prepareCall("{call SP_GESZPJITEM(?,?,?,?,?,?)}");
        call.setString(1,DOMAIN_ID);
        call.setString(2,TYPEID);
        call.setString(3,MONITORID);
        call.setString(4,MONITORSIDE);
        call.setString(5, date);
        call.registerOutParameter(6, OracleTypes.CURSOR);
        call.execute();
        ResultSet rs= (ResultSet)call.getObject(6);
        while (rs.next()){
            ZbValue zbValue = new ZbValue();
            zbValue.setITEM_ID(rs.getString(1));
            zbValue.setSYS_ID(rs.getString(2));
            zbValue.setITEM_NAME(rs.getString(3));
            zbValue.setITEM_VALUE(rs.getString(4)==null?"":rs.getString(4));
            zbValue.setRemark(rs.getString(5)==null?"":rs.getString(5));
            zbValues.add(zbValue);
        }
        rs.close();
        call.close();
        connection.close();
        return zbValues;
    }

    @Override
    public List<QyPoint> getQyPoint() throws Exception{
        List<QyPoint> qyPoints = new ArrayList<QyPoint>();
        Connection connection = basicDataSource.getConnection();
        String sql = "select t.取样位置_id,t.取样位置 from szpj_sys_取样位置表 t ORDER BY t.取样位置_id";
        PreparedStatement ps = connection.prepareStatement(sql);
        ResultSet  rs = ps.executeQuery();
        while (rs.next()){
            QyPoint qyPoint = new QyPoint();
            qyPoint.setId(rs.getString(1));
            qyPoint.setPoint(rs.getString(2));
            qyPoints.add(qyPoint);
        }
        rs.close();
        ps.close();
        connection.close();
        return qyPoints;
    }

    @Override
    public void saveSzpj(long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            List<ZbValue> zbValues = (List<ZbValue>)JsonUtil.readJson(addData,ArrayList.class,ZbValue.class);
            String sql2 = null;
            if(!"".equals(addData)){
                for(int i = 0; i < zbValues.size();i++){
                    sql2 = "insert into szpj_biz_水质指标观测数据 (SN_ID,观测点_ID,观测时间,观测ITEM_ID,观测值,备注,取样位置) values(sys_guid(),?,?,?,?,?,?)";
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1,placeId);
                    ps.setTimestamp(2, new Timestamp(gcTime));
                    ps.setString(3,zbValues.get(i).getITEM_ID());
                    ps.setString(4,zbValues.get(i).getITEM_VALUE());
                    ps.setString(5,zbValues.get(i).getRemark());
                    ps.setString(6,qyPoint);
                    ps.execute();
                }
            }

            String sql1 = "insert into szpj_biz_水文观测数据 (SN_ID,观测点_ID,观测时间,水位,流量,水温,气温,备注,取样位置) values(sys_guid(),?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, placeId);
            ps.setTimestamp(2, new Timestamp(gcTime));
            ps.setString(3,shuiwei);
            ps.setString(4, liuliang);
            ps.setString(5,shuiwen);
            ps.setString(6,qiwen);
            ps.setString(7,remark);
            ps.setString(8, qyPoint);
            ps.execute();

            connection.commit();

        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
            try {
                ps.close();
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }

    }

    @Override
    public void deleteSzpj(Long gcTime, String placeId, String qyPoint) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String time =sdf.format(new Date(gcTime));
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from szpj_biz_水文观测数据 where 观测点_ID = ? and 观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss') and 取样位置 = ?";
            String sql2 = "delete from szpj_biz_水质指标观测数据 where 观测点_ID = ? and 观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss') and 取样位置 = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, placeId);
            ps.setString(2, time);
            ps.setString(3, qyPoint);
            ps.execute();

            ps = connection.prepareStatement(sql2);
            ps.setString(1, placeId);
            ps.setString(2, time);
            ps.setString(3, qyPoint);
            ps.execute();
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
    }

    @Override
    public void updateSzpj(String formid,Long gcTime, String placeId, String qyPoint, String shuiwei, String liuliang, String shuiwen, String qiwen, String remark, String editData,String addData) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date =sdf.format(new Date(gcTime));
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            //通过formid找到szpj_biz_水文观测数据表中原先的数据：观测点_ID ，观测时间，取样位置
            String sql = "select 观测点_ID,观测时间,取样位置 from szpj_biz_水文观测数据 where SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,formid);
            rs = ps.executeQuery();
            if (rs.next()){
                //数据库里边的 观测点id，观测时间，取样位置
                String gcId = null;
                String time = null;
                String point = null;
                gcId = rs.getString("观测点_ID");
                time =sdf.format(new Date(rs.getTimestamp("观测时间").getTime()));
                point = rs.getString("取样位置");

                //开始更新数据库信息，首先更新   szpj_biz_水文观测数据  表中的数据
                String sql1 = "update szpj_biz_水文观测数据 set 观测点_ID = ?,观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss'),水位 = ?,流量 = ?,水温 = ?,气温 = ?,备注 = ?,取样位置 = ?  where SN_ID = ?";
                ps = connection.prepareStatement(sql1);
                ps.setString(1,placeId);
                ps.setString(2, date);
                ps.setString(3, shuiwei);
                ps.setString(4,liuliang);
                ps.setString(5,shuiwen);
                ps.setString(6,qiwen);
                ps.setString(7,remark);
                ps.setString(8,qyPoint);
                ps.setString(9,formid);
                ps.execute();

                if(editData!=null && !"".equals(editData)){
                    List<ZbValue> zbValues = (List<ZbValue>)JsonUtil.readJson(editData,ArrayList.class,ZbValue.class);
                    for(int i = 0; i < zbValues.size(); i++){
                        String sql2  = "update szpj_biz_水质指标观测数据 set 观测点_ID = ?,观测时间 = ?,观测ITEM_ID = ?,观测值 = ?,备注 = ? ,取样位置 = ?  where 观测点_ID = ? and 观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss') and 取样位置 = ? and 观测ITEM_ID = ?";
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1,placeId);
                        ps.setTimestamp(2, new Timestamp(gcTime));
                        ps.setString(3, zbValues.get(i).getITEM_ID());
                        ps.setString(4,zbValues.get(i).getITEM_VALUE());
                        ps.setString(5,zbValues.get(i).getRemark());
                        ps.setString(6, qyPoint);
                        ps.setString(7, gcId);
                        ps.setString(8, time);
                        ps.setString(9, point);
                        ps.setString(10, zbValues.get(i).getITEM_ID());
                        ps.execute();
                    }
                }
                if(addData!=null && !"".equals(addData)){
                    //以下这两个获取list集合的方法都可用
//                    List<ZbValue> addZbValues = (List<ZbValue>)JsonUtil.readJson(addData,ArrayList.class,ZbValue.class);
                    List<ZbValue> addZbValues = (List<ZbValue>)JsonUtil.readValue(addData, new TypeReference<ArrayList<ZbValue>>() {
                    });
                    for(int i = 0; i < addZbValues.size(); i++){
                        String sql3 = "insert into szpj_biz_水质指标观测数据 (SN_ID,观测点_ID,观测时间,观测ITEM_ID,观测值,备注,取样位置) values(sys_guid(),?,?,?,?,?,?)";
                        ps = connection.prepareStatement(sql3);
                        ps.setString(1,placeId);
                        ps.setTimestamp(2, new Timestamp(gcTime));
                        ps.setString(3, addZbValues.get(i).getITEM_ID());
                        ps.setString(4, addZbValues.get(i).getITEM_VALUE());
                        ps.setString(5, addZbValues.get(i).getRemark());
                        ps.setString(6,qyPoint);
                        ps.execute();
                    }

                }

            }
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
                rs.close();
            ps.close();
            connection.close();
        }
    }
//排污口水质监测
    @Override
    public List<Places> getPwPlaces(String waterBodyId,String keywords) throws Exception{
        if("".equals(keywords)){
            keywords=null;
        }
        List<Places> places = new ArrayList<Places>();
        Connection connection = basicDataSource.getConnection();
        CallableStatement call = connection.prepareCall("{call SP_GETMONITOROBJ(?,?,?,?)}");
        call.setString(1, waterBodyId);
        call.setString(2,"排污口");
        call.setString(3, keywords);
        call.registerOutParameter(4, OracleTypes.CURSOR);
        call.execute();
        ResultSet rs= (ResultSet)call.getObject(4);
        while (rs.next()){
            Places place = new Places();
            place.setObj_id(rs.getString(1));
            place.setType(rs.getString(2));
            place.setName(rs.getString(3));
            places.add(place);
        }
        rs.close();
        call.close();
        connection.close();
        return places;
    }

    @Override
    public void savePwkSzpj(long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData,String paifang) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            List<ZbValue> zbValues = (List<ZbValue>)JsonUtil.readJson(addData,ArrayList.class,ZbValue.class);
            String sql2 = null;
            if(!"".equals(addData)){
                for(int i = 0; i < zbValues.size();i++){
                    sql2 = "insert into szpj_biz_水质指标观测数据 (SN_ID,观测点_ID,观测时间,观测ITEM_ID,观测值,备注,取样位置) values(sys_guid(),?,?,?,?,?,?)";
                    ps = connection.prepareStatement(sql2);
                    ps.setString(1, placeId);
                    ps.setTimestamp(2, new Timestamp(gcTime));
                    ps.setString(3,zbValues.get(i).getITEM_ID());
                    ps.setString(4,zbValues.get(i).getITEM_VALUE());
                    ps.setString(5,zbValues.get(i).getRemark());
                    ps.setString(6,qyPoint);
                    ps.execute();
                }
            }

            String sql1 = "insert into szpj_biz_水文观测数据 (SN_ID,观测点_ID,观测时间,水位,流量,水温,气温,备注,取样位置,排放规律) values(sys_guid(),?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, placeId);
            ps.setTimestamp(2, new Timestamp(gcTime));
            ps.setString(3, shuiwei);
            ps.setString(4,liuliang);
            ps.setString(5,shuiwen);
            ps.setString(6,qiwen);
            ps.setString(7,remark);
            ps.setString(8, qyPoint);
            ps.setString(9,paifang);
            ps.execute();

            connection.commit();

        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
            try {
                ps.close();
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            }
        }

    }

    @Override
    public void updatePwkSzpj(String formid,Long gcTime, String placeId, String qyPoint, String shuiwei, String liuliang, String shuiwen, String qiwen, String remark, String editData,String addData,String paifang) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date =sdf.format(new Date(gcTime));
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            //通过formid找到szpj_biz_水文观测数据表中原先的数据：观测点_ID ，观测时间，取样位置
            String sql = "select 观测点_ID,观测时间,取样位置 from szpj_biz_水文观测数据 where SN_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1, formid);
            rs = ps.executeQuery();
            if (rs.next()){
                //数据库里边的 观测点id，观测时间，取样位置
                String gcId = null;
                String time = null;
                String point = null;
                gcId = rs.getString("观测点_ID");
                time =sdf.format(new Date(rs.getTimestamp("观测时间").getTime()));
                point = rs.getString("取样位置");

                //开始更新数据库信息，首先更新   szpj_biz_水文观测数据  表中的数据
                String sql1 = "update szpj_biz_水文观测数据 set 观测点_ID = ?,观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss'),水位 = ?,流量 = ?,水温 = ?,气温 = ?,备注 = ?,取样位置 = ?,排放规律 = ?  where SN_ID = ?";
                ps = connection.prepareStatement(sql1);
                ps.setString(1,placeId);
                ps.setString(2, date);
                ps.setString(3, shuiwei);
                ps.setString(4,liuliang);
                ps.setString(5,shuiwen);
                ps.setString(6,qiwen);
                ps.setString(7,remark);
                ps.setString(8,qyPoint);
                ps.setString(9,paifang);
                ps.setString(10,formid);
                ps.execute();

                if(editData!=null && !"".equals(editData)){
                    List<ZbValue> zbValues = (List<ZbValue>)JsonUtil.readJson(editData,ArrayList.class,ZbValue.class);
                    for(int i = 0; i < zbValues.size(); i++){
                        String sql2  = "update szpj_biz_水质指标观测数据 set 观测点_ID = ?,观测时间 = ?,观测ITEM_ID = ?,观测值 = ?,备注 = ? ,取样位置 = ? where 观测点_ID = ? and 观测时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss') and 取样位置 = ? and 观测ITEM_ID = ?";
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1, placeId);
                        ps.setTimestamp(2, new Timestamp(gcTime));
                        ps.setString(3, zbValues.get(i).getITEM_ID());
                        ps.setString(4,zbValues.get(i).getITEM_VALUE());
                        ps.setString(5,zbValues.get(i).getRemark());
                        ps.setString(6, qyPoint);
                        ps.setString(7, gcId);
                        ps.setString(8, time);
                        ps.setString(9, point);
                        ps.setString(10, zbValues.get(i).getITEM_ID());
                        ps.execute();
                    }
                }
                if(addData!=null && !"".equals(addData)){
                    //以下这两个获取list集合的方法都可用
//                    List<ZbValue> addZbValues = (List<ZbValue>)JsonUtil.readJson(addData,ArrayList.class,ZbValue.class);
                    List<ZbValue> addZbValues = (List<ZbValue>)JsonUtil.readValue(addData, new TypeReference<ArrayList<ZbValue>>() {
                    });
                    for(int i = 0; i < addZbValues.size(); i++){
                        String sql3 = "insert into szpj_biz_水质指标观测数据 (SN_ID,观测点_ID,观测时间,观测ITEM_ID,观测值,备注,取样位置) values(sys_guid(),?,?,?,?,?,?)";
                        ps = connection.prepareStatement(sql3);
                        ps.setString(1, placeId);
                        ps.setTimestamp(2, new Timestamp(gcTime));
                        ps.setString(3, addZbValues.get(i).getITEM_ID());
                        ps.setString(4, addZbValues.get(i).getITEM_VALUE());
                        ps.setString(5, addZbValues.get(i).getRemark());
                        ps.setString(6,qyPoint);
                        ps.execute();
                    }

                }
            }
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
            rs.close();
            ps.close();
            connection.close();
        }
    }

    @Override
    public List<Map<String, String>> getWrwZbValue(String MONITORID,String MONITORDATE) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String date = null;
        if(MONITORDATE!=null&&(!"".equals(MONITORDATE))){
            date = sdf.format(new Date(Long.valueOf(MONITORDATE)));
        }else{
            date = null;
        }
        List<Map<String,String>> zbValues = new ArrayList<Map<String,String>>();
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall("{call SP_GETWRWITEM(?,?,?)}");
            call.setString(1,MONITORID);
            call.setString(2, date);
            call.registerOutParameter(3, OracleTypes.CURSOR);
            call.execute();
            rs = (ResultSet) call.getObject(3);
            while (rs.next()){
                Map<String,String> zbValue = new HashMap<String,String>();
                zbValue.put("ITEM_ID",rs.getString("ITEM_ID"));
                zbValue.put("ITEM_NAME",rs.getString("ITEM_NAME"));
                zbValue.put("ITEM_VALUE",rs.getString("ITEM_VALUE"));
                zbValues.add(zbValue);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            rs.close();
            call.close();
            connection.close();
        }
        return zbValues;
    }

    @Override
    public void saveSzy(Long jcTime, String waterFunctionID, String addData) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            List<Map<String,Object>> list = JsonUtil.readJson(addData,ArrayList.class,HashMap.class);
            for(int i = 0 ; i < list.size() ; i++){
                String sql = "insert into szpj_biz_排污口污染物入河量 (SN_ID,监测日期,排污口_ID,污染物指标_ID,污染物入河量 ) values(sys_guid(),?,?,?,?)";
                ps = connection.prepareStatement(sql);
                ps.setDate(1, new java.sql.Date(jcTime));
                ps.setString(2, waterFunctionID);
                ps.setString(3, (String) list.get(i).get("item_ID"));
                ps.setObject(4, list.get(i).get("item_VALUE"));
                ps.execute();
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
    }

    @Override
    public void deleteSzy(Long jcTime, String waterFunctionID) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from szpj_biz_排污口污染物入河量 where 监测日期 = ? and 排污口_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setDate(1,new java.sql.Date(jcTime));
            ps.setString(2, waterFunctionID);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
    }

    @Override
    public void updateSzy(Long jcTime, String waterFunctionID, String editData, String beforeJcTime, String beforeWaterFounctionID) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update szpj_biz_排污口污染物入河量 set 监测日期 = ?,排污口_ID = ? where 监测日期 = to_date(?,'yyyy-MM-dd') and 排污口_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setDate(1,new java.sql.Date(jcTime));
            ps.setString(2, waterFunctionID);
            ps.setString(3, beforeJcTime);
            ps.setString(4, beforeWaterFounctionID);
            ps.execute();
            ps.close();

            if(editData!=null && !"".equals(editData)){
                List<Map<String,Object>> list = JsonUtil.readJson(editData,ArrayList.class,HashMap.class);
                for(int i = 0 ; i < list.size() ; i++){
                    String sql1 = "update szpj_biz_排污口污染物入河量 set 污染物入河量 = ? where 监测日期 = to_date(?,'yyyy-MM-dd') and 排污口_ID = ? and 污染物指标_ID = ?";
                    ps = connection.prepareStatement(sql1);
                    ps.setObject(1, list.get(i).get("item_VALUE"));
                    ps.setString(2, beforeJcTime);
                    ps.setString(3, beforeWaterFounctionID);
                    ps.setString(4, (String) list.get(i).get("item_ID"));
                    ps.execute();
                }

            }

        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
    }
}
