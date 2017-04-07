package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.dao.EmergencyManagerDao;
import bitc.sz.wzEnv.util.JsonUtil;
import bitc.sz.wzEnv.util.Md5Util;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import javax.servlet.http.HttpServletRequest;
import javax.sql.DataSource;
import java.io.*;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Administrator on 2016/9/9.
 */
@Repository
public class EmergencyManagerDaoImpl implements EmergencyManagerDao {
    @Autowired
    private DataSource basicDataSource;

    @Override
    public List<Map<String, Object>> queryWrwMessage(String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,Object>> endList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = " select 污染物_ID from tfsj_sys_污染物字典表";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            Set<String> wrwIdSet = new HashSet<String>();
            while (rs.next()){
                wrwIdSet.add(rs.getString(1));
            }
            String sql2 = "select t2.污染物_id, t1.item_name, t2.item_values, t1.序号\n" +
                    "  from (select *\n" +
                    "          from (select t.*, rownum rownum_\n" +
                    "                  from (select * from tfsj_sys_污染物属性表 order by 序号) t\n" +
                    "                 where rownum < 4)) t1\n" +
                    "  left join tfsj_sys_污染物字典表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and t2.污染物_id = ?\n";
            Iterator wrwIds = wrwIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
            while (wrwIds.hasNext()){
                String currentWrwId = (String) wrwIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1, currentWrwId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("wrwId", currentWrwId);
                while (rs.next()){
                    propertyNameSet.add(rs.getString(2));
                    resultMap.put(rs.getString(2), rs.getString(3));
                }
                tempList.add(resultMap);
            }
            if(!"".equals(keyword) && keyword != null){
                for(int i = 0 ; i < tempList.size() ; i++){
                    Map<String,String> map = tempList.get(i);
                    boolean flag = false;
                    Iterator propertyNames = propertyNameSet.iterator();
                    while (propertyNames.hasNext()){
                        String currentPropertyName = (String) propertyNames.next();
                        if(StringUtils.isNotEmpty(map.get(currentPropertyName))){
                            if(map.get(currentPropertyName).contains(keyword)){
                                flag = true;
                                break;
                            }
                        }
                    }
                    if(flag){
                        resultList.add(map);
                    }
                }
            }else{
                resultList = tempList;
            }
            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> endMap = new LinkedHashMap<String,Object>();
                Map<String,String> map = resultList.get(i);
                List<Map<String,String>> currentList = new ArrayList<Map<String, String>>();
                Set<String> keyNameSet= map.keySet();
                Iterator keyNames = keyNameSet.iterator();
                while (keyNames.hasNext()){
                    String currentKeyName = (String) keyNames.next();
                    Map<String,String> currentMap = new LinkedHashMap<String,String>();
                    if("wrwId".equals(currentKeyName)){
                        endMap.put("wrwId",map.get(currentKeyName));
                        continue;
                    }
                    currentMap.put("itemName",currentKeyName);
                    currentMap.put("itemValue", map.get(currentKeyName));
                    currentList.add(currentMap);
                }
                endMap.put("attribute",currentList);
                endList.add(endMap);
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
        return endList;
    }

    @Override
    public List<Map<String, Object>> queryFxyMessage(String water_body_id,String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,Object>> endList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = " select distinct t1.风险源_id\n" +
                    "  from tfsj_sys_风险源字典表 t1, tfsj_rel_应急信息水体关系表 t2\n" +
                    " where t1.风险源_id = t2.tfsj_item_id\n" +
                    "   and t2.water_body_id = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            Set<String> fxyIdSet = new HashSet<String>();
            while (rs.next()){
                fxyIdSet.add(rs.getString(1));
            }
            String sql2 = " select t2.风险源_id, t1.item_name, t2.item_values, t1.序号\n" +
                    "   from (select *\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tfsj_sys_风险源属性表 order by 序号) t\n" +
                    "         where rownum < 4)) t1\n" +
                    "   left join tfsj_sys_风险源字典表 t2\n" +
                    "     on t1.item_id = t2.item_id\n" +
                    "    and t2.风险源_id = ?\n";
            Iterator fxyIds = fxyIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
            while (fxyIds.hasNext()){
                String currentFxyId = (String) fxyIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1, currentFxyId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("fxyId", currentFxyId);
                while (rs.next()){
                    propertyNameSet.add(rs.getString(2));
                    resultMap.put(rs.getString(2),rs.getString(3));
                }
                String sql3 = "select 经纬度坐标 from tfsj_sys_风险源坐标 where 风险源_ID = ?";
                ps = connection.prepareStatement(sql3);
                ps.setString(1, currentFxyId);
                rs = ps.executeQuery();
                if (rs.next()){
                    resultMap.put("经纬度坐标",rs.getString(1));
                }
                tempList.add(resultMap);
            }
            if(!"".equals(keyword) && keyword != null){
                for(int i = 0 ; i < tempList.size() ; i++){
                    Map<String,String> map = tempList.get(i);
                    boolean flag = false;
                    Iterator propertyNames = propertyNameSet.iterator();
                    while (propertyNames.hasNext()){
                        String currentPropertyName = (String) propertyNames.next();
                        if(StringUtils.isNotEmpty(map.get(currentPropertyName))){
                            if(map.get(currentPropertyName).contains(keyword)){
                                flag = true;
                                break;
                            }
                        }
                    }
                    if(flag){
                        resultList.add(map);
                    }
                }
            }else{
                resultList = tempList;
            }
            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> endMap = new LinkedHashMap<String,Object>();
                Map<String,String> map = resultList.get(i);
                List<Map<String,String>> currentList = new ArrayList<Map<String, String>>();
                Set<String> keyNameSet= map.keySet();
                Iterator keyNames = keyNameSet.iterator();
                while (keyNames.hasNext()){
                    String currentKeyName = (String) keyNames.next();
                    Map<String,String> currentMap = new LinkedHashMap<String,String>();
                    if("fxyId".equals(currentKeyName)){
                        endMap.put("fxyId",map.get(currentKeyName));
                        continue;
                    }
                    currentMap.put("itemName",currentKeyName);
                    currentMap.put("itemValue", map.get(currentKeyName));
                    currentList.add(currentMap);
                }
                endMap.put("attribute",currentList);
                endList.add(endMap);
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
        return endList;
    }

    @Override
    public List<Map<String, Object>> queryZjMessage(String water_body_id,String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,Object>> endList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select distinct t1.专家_id\n" +
                    "  from tfsj_sys_专家库表 t1, tfsj_rel_应急信息水体关系表 t2\n" +
                    " where t1.专家_id = t2.tfsj_item_id\n" +
                    "   and t2.water_body_id = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            Set<String> zjIdSet = new HashSet<String>();
            while (rs.next()){
                zjIdSet.add(rs.getString(1));
            }
            String sql2 = " select t2.专家_ID, t1.item_name, t2.item_values, t1.序号\n" +
                    "   from (select *\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tsfj_sys_专家库属性表 order by 序号) t\n" +
                    "         where rownum < 4)) t1\n" +
                    "   left join tfsj_sys_专家库表 t2\n" +
                    "     on t1.item_id = t2.item_id\n" +
                    "    and t2.专家_ID = ?\n";
            Iterator zjIds = zjIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
            while (zjIds.hasNext()){
                String currentZjId = (String) zjIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1, currentZjId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("zjId", currentZjId);
                while (rs.next()){
                    propertyNameSet.add(rs.getString(2));
                    resultMap.put(rs.getString(2),rs.getString(3));
                }
                tempList.add(resultMap);
            }
            if(!"".equals(keyword) && keyword != null){
                for(int i = 0 ; i < tempList.size() ; i++){
                    Map<String,String> map = tempList.get(i);
                    boolean flag = false;
                    Iterator propertyNames = propertyNameSet.iterator();
                    while (propertyNames.hasNext()){
                        String currentPropertyName = (String) propertyNames.next();
                        if(StringUtils.isNotEmpty(map.get(currentPropertyName))){
                            if(map.get(currentPropertyName).contains(keyword)){
                                flag = true;
                                break;
                            }
                        }
                    }
                    if(flag){
                        resultList.add(map);
                    }
                }
            }else{
                resultList = tempList;
            }
            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> endMap = new LinkedHashMap<String,Object>();
                Map<String,String> map = resultList.get(i);
                List<Map<String,String>> currentList = new ArrayList<Map<String, String>>();
                Set<String> keyNameSet= map.keySet();
                Iterator keyNames = keyNameSet.iterator();
                while (keyNames.hasNext()){
                    String currentKeyName = (String) keyNames.next();
                    Map<String,String> currentMap = new LinkedHashMap<String,String>();
                    if("zjId".equals(currentKeyName)){
                        endMap.put("zjId",map.get(currentKeyName));
                        continue;
                    }
                    currentMap.put("itemName",currentKeyName);
                    currentMap.put("itemValue", map.get(currentKeyName));
                    currentList.add(currentMap);
                }
                endMap.put("attribute",currentList);
                endList.add(endMap);
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
        return endList;
    }

    @Override
    public List<Map<String, Object>> queryYuanMessage(String water_body_id,String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        List<Map<String,Object>> endList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t1.应急预案_id, t1.应急预案名称, t1.编制日期\n" +
                    "  from tfsj_biz_应急预案法规表 t1, tfsj_rel_应急信息水体关系表 t2\n" +
                    " where t1.应急预案_id = t2.tfsj_item_id\n" +
                    "   and t1.type_id = '1'\n" +
                    "   and t2.water_body_id = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                if(StringUtils.isNotEmpty(keyword)){
                    if(StringUtils.isNotEmpty(rs.getString(2))&&rs.getString(2).contains(keyword)){
                        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                        resultMap.put("planId",rs.getString(1));
                        resultMap.put("预案名称",rs.getString(2));
                        resultMap.put("编制日期",rs.getDate(3));
                        resultList.add(resultMap);
                    }
                }else{
                    Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                    resultMap.put("planId",rs.getString(1));
                    resultMap.put("预案名称",rs.getString(2));
                    resultMap.put("编制日期",rs.getDate(3));
                    resultList.add(resultMap);
                }
            }
            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> map = resultList.get(i);
                Set<String> keySet = map.keySet();
                Iterator keyNames = keySet.iterator();
                Map<String,Object> endMap = new LinkedHashMap<String,Object>();
                List<Map<String,Object>> tempList = new ArrayList<Map<String, Object>>();
                while (keyNames.hasNext()){
                    String currentKeyName = (String) keyNames.next();
                    if("planId".equals(currentKeyName)){
                        endMap.put(currentKeyName,map.get(currentKeyName));
                    }else{
                        Map<String,Object> child = new LinkedHashMap<String,Object>();
                        child.put("itemName",currentKeyName);
                        child.put("itemValue", map.get(currentKeyName));
                        tempList.add(child);
                    }
                }
                endMap.put("attribute",tempList);
                endList.add(endMap);
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
        return endList;
    }

    @Override
    public List<Map<String, Object>> queryFaguiMessage(String water_body_id,String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        List<Map<String,Object>> endList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t1.应急预案_id, t1.应急预案名称, t1.编制日期\n" +
                    "  from tfsj_biz_应急预案法规表 t1, tfsj_rel_应急信息水体关系表 t2\n" +
                    " where t1.应急预案_id = t2.tfsj_item_id\n" +
                    "   and t1.type_id = '2'\n" +
                    "   and t2.water_body_id = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                if(!"".equals(keyword) && keyword != null){
                    if(StringUtils.isNotEmpty(rs.getString(2))&&rs.getString(2).contains(keyword)){
                        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                        resultMap.put("lawId",rs.getString(1));
                        resultMap.put("法规名称",rs.getString(2));
                        resultMap.put("编制日期",rs.getDate(3));
                        resultList.add(resultMap);
                    }
                }else{
                    Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                    resultMap.put("lawId",rs.getString(1));
                    resultMap.put("法规名称",rs.getString(2));
                    resultMap.put("编制日期",rs.getDate(3));
                    resultList.add(resultMap);
                }
            }
            for(int i = 0 ; i < resultList.size() ; i++){
                Map<String,Object> map = resultList.get(i);
                Set<String> keySet = map.keySet();
                Iterator keyNames = keySet.iterator();
                Map<String,Object> endMap = new LinkedHashMap<String,Object>();
                List<Map<String,Object>> tempList = new ArrayList<Map<String, Object>>();
                while (keyNames.hasNext()){
                    String currentKeyName = (String) keyNames.next();
                    if("lawId".equals(currentKeyName)){
                        endMap.put(currentKeyName,map.get(currentKeyName));
                    }else{
                        Map<String,Object> child = new LinkedHashMap<String,Object>();
                        child.put("itemName",currentKeyName);
                        child.put("itemValue", map.get(currentKeyName));
                        tempList.add(child);
                    }
                }
                endMap.put("attribute",tempList);
                endList.add(endMap);
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
        return endList;
    }

    @Override
    public List<Map<String, Object>> queryWaterPollutionEventCoordinate(String water_body_id, String keyword) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        try {
            connection = basicDataSource.getConnection();
            StringBuilder sql = new StringBuilder();
            int count = 0 ;
            sql.append("select  污染事件_ID,污染事件名称,开始时间,结束时间,经纬度坐标\n" +
                    "  from tfsj_biz_水污染事件\n" +
                    " where WATER_BODY_ID = ?");
            if(StringUtils.isNotEmpty(keyword)){
                sql.append(" and 污染事件名称 like ?\n" +
                        "    or 发生地点 like ?\n" +
                        "    or 事件描述 like ?");
                Pattern pattern1 = Pattern.compile("(\\d{4})(\\.)(\\d{1})");
                Matcher matcher1 = pattern1.matcher(keyword);

                Pattern pattern2 = Pattern.compile("(\\d{4})(-)(\\d{1})");
                Matcher matcher2 = pattern2.matcher(keyword);

                Pattern pattern3 = Pattern.compile("^[-\\+]?[\\d]*$");
                Matcher matcher3 = pattern3.matcher(keyword);

                if(matcher1.find()){
                    sql.append(" or to_date(?, 'yyyy.mm') >= 开始时间\n" +
                            "   and to_date(?, 'yyyy.mm') <= 结束时间");
                }else if(matcher2.find()){
                    sql.append(" or to_date(?, 'yyyy-mm') >= 开始时间\n" +
                            "   and to_date(?, 'yyyy-mm') <= 结束时间");
                }else if(matcher3.find()&&keyword.length()==4){
                    sql.append(" or to_date(?, 'yyyy') >= 开始时间\n" +
                            "   and to_date(?, 'yyyy') <= 结束时间");
                }
                String SQL = null;
                SQL = sql.toString();
                char[] arr  = SQL.toCharArray();

                for(int i = 0 ; i < arr.length ; i++){
                    if('?'==arr[i]){
                        count++;
                    }
                }
            }
            ps = connection.prepareStatement(sql.toString());
            ps.setString(1,water_body_id);
            if(count==4){
                ps.setString(2,"%"+keyword+"%");
                ps.setString(3,"%"+keyword+"%");
                ps.setString(4,"%"+keyword+"%");
            }
            if(count==6){
                ps.setString(2,"%"+keyword+"%");
                ps.setString(3,"%"+keyword+"%");
                ps.setString(4,"%"+keyword+"%");
                ps.setString(5,keyword);
                ps.setString(6,keyword);
            }
            rs = ps.executeQuery();
            while(rs.next()){
                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                resultMap.put("pollutionEventID",rs.getString(1));
                resultMap.put("pollutionEventName",rs.getString(2));
                resultMap.put("startTime",sdf.format(new Date(rs.getTimestamp(3).getTime())));
                resultMap.put("endTime",sdf.format(new Date(rs.getTimestamp(4).getTime())));
                resultMap.put("coordinate",rs.getString(5));
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
    public List<Map<String, String>> queryWrwMessageById(String wrwId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql2 = "select t1.item_name, t2.item_values\n" +
                    "  from tfsj_sys_污染物属性表 t1\n" +
                    "  left join tfsj_sys_污染物字典表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and 污染物_id = ?\n" +
                    " order by t1.序号";
            ps = connection.prepareStatement(sql2);
            ps.setString(1, wrwId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("itemName", rs.getString(1));
                resultMap.put("itemValue", rs.getString(2));
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
    public List<Map<String, String>> queryFxyMessageById(String fxyId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql2 = "select t1.item_name, t2.item_values\n" +
                    "  from tfsj_sys_风险源属性表 t1\n" +
                    "  left join tfsj_sys_风险源字典表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and 风险源_id = ?\n" +
                    " order by t1.序号";
            ps = connection.prepareStatement(sql2);
            ps.setString(1, fxyId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("itemName", rs.getString(1));
                resultMap.put("itemValue", rs.getString(2));
                resultList.add(resultMap);
            }
            String sql3 = "select 经纬度坐标 from tfsj_sys_风险源坐标 where 风险源_ID = ?";
            ps = connection.prepareStatement(sql3);
            ps.setString(1, fxyId);
            rs = ps.executeQuery();
            if (rs.next()){
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("itemName","经纬度坐标");
                resultMap.put("itemValue",rs.getString(1));
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
    public List<Map<String, String>> queryZjMessageById(String zjId,HttpServletRequest request) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql2 = "select t1.item_name, t2.item_values\n" +
                    "  from tsfj_sys_专家库属性表 t1\n" +
                    "  left join tfsj_sys_专家库表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and 专家_ID = ?\n" +
                    " order by t1.序号";
            ps = connection.prepareStatement(sql2);
            ps.setString(1, zjId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("itemName", rs.getString(1));
                resultMap.put("itemValue", rs.getString(2));
                resultList.add(resultMap);
            }
            String sql3 = "select 照片,上传文件名 from tfsj_sys_专家照片 where 专家_ID = ?";
            ps = connection.prepareStatement(sql3);
            ps.setString(1, zjId);
            rs = ps.executeQuery();
            if (rs.next()){
                String path = request.getSession().getServletContext().getRealPath("");
                String photoPath = path+"\\download\\"+rs.getString(2);
                Blob blob = rs.getBlob(1);
                InputStream in = blob.getBinaryStream();
                BufferedInputStream inputStream = new BufferedInputStream(in);
                File file = new File(photoPath);
                if(!file.getParentFile().isFile()){
                    file.getParentFile().mkdirs();
                }
                OutputStream outputStream = new FileOutputStream(file);
                int i = 0;
                while ((i = inputStream.read()) != -1){
                    outputStream.write(i);
                }
                inputStream.close();
                outputStream.flush();
                outputStream.close();
                Map<String,String> resultMap = new LinkedHashMap<String,String>();
                resultMap.put("itemName","照片路径");
                String url = request.getContextPath();
                resultMap.put("itemValue", url+"/download/"+rs.getString(2));
                resultList.add(resultMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
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
    public List<Map<String, Object>> queryYuanMessageById(String planId,HttpServletRequest request) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 应急预案名称,编制日期,附件_OBJ,上传文件名 from tfsj_biz_应急预案法规表 where TYPE_ID = 1 and 应急预案_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs = ps.executeQuery();
            Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
            if (rs.next()){
                resultMap.put("应急预案名称",rs.getString(1));
                resultMap.put("编制日期",rs.getDate(2));

                String path = request.getSession().getServletContext().getRealPath("");
                String filePath = path+"\\download\\"+rs.getString(4);
                Blob blob = rs.getBlob(3);
                InputStream in = blob.getBinaryStream();
                BufferedInputStream inputStream = new BufferedInputStream(in);
                File file = new File(filePath);
                if(!file.getParentFile().isFile()){
                    file.getParentFile().mkdirs();
                }
                OutputStream outputStream = new FileOutputStream(file);
                int i = 0;
                while ((i = inputStream.read()) != -1){
                    outputStream.write(i);
                }
                inputStream.close();
                outputStream.flush();
                outputStream.close();
                String url =  request.getContextPath();
                resultMap.put("附件路径", url+"/download/"+rs.getString(4));
            }
            Set<String> keySet = resultMap.keySet();
            Iterator keyNames = keySet.iterator();
            while (keyNames.hasNext()){
                String currentName = (String) keyNames.next();
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                map.put("itemName",currentName);
                map.put("itemValue",resultMap.get(currentName));
                resultList.add(map);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
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
    public List<Map<String, Object>> queryFaguiMessageById(String lawId, HttpServletRequest request) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 应急预案名称,编制日期,附件_OBJ,上传文件名 from tfsj_biz_应急预案法规表 where TYPE_ID = 2 and 应急预案_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,lawId);
            rs = ps.executeQuery();
            Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
            if (rs.next()){
                resultMap.put("应急法规名称",rs.getString(1));
                resultMap.put("编制日期",rs.getDate(2));

                String path = request.getSession().getServletContext().getRealPath("");
                String filePath = path+"\\download\\"+rs.getString(4);
                Blob blob = rs.getBlob(3);
                InputStream in = blob.getBinaryStream();
                BufferedInputStream inputStream = new BufferedInputStream(in);
                File file = new File(filePath);
                if(!file.getParentFile().isFile()){
                    file.getParentFile().mkdirs();
                }
                OutputStream outputStream = new FileOutputStream(file);
                int i = 0;
                while ((i = inputStream.read()) != -1){
                    outputStream.write(i);
                }
                inputStream.close();
                outputStream.flush();
                outputStream.close();
                String url =  request.getContextPath();
                resultMap.put("附件路径", url+"/download/"+rs.getString(4));
            }
            Set<String> keySet = resultMap.keySet();
            Iterator keyNames = keySet.iterator();
            while (keyNames.hasNext()){
                String currentName = (String) keyNames.next();
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                map.put("itemName",currentName);
                map.put("itemValue",resultMap.get(currentName));
                resultList.add(map);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
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
    public void saveWaterPollutionEvent(String pollutedJSON) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            Map<String,Object> map = JsonUtil.readValue(pollutedJSON, Map.class);
            String wrwEventId = (String) map.get("eventId");
            String water_body_id = (String) map.get("waterbodyid");
            String eventNm = (String) map.get("eventName");
            String startTime = (String) map.get("startTime");
            String endTime = (String) map.get("endTime");
            String place = (String) map.get("place");
            String side = (String) map.get("side");
            String longitude = (String) map.get("longtude");
            String latitude = (String) map.get("Latiude");
            String describe = (String) map.get("describe");
            List<Map<String,String>> pollutedInfo = null;
            if(map.get("wrwInfo") != null){
                pollutedInfo = (List<Map<String, String>>) map.get("wrwInfo");
            }
            if(StringUtils.isEmpty(wrwEventId)){
                //增加
                String uuid = Md5Util.getUUID();
                String sql1 = "insert into tfsj_biz_水污染事件(污染事件_ID,WATER_BODY_ID,污染事件名称,开始时间,结束时间,发生地点,位置_ID,经纬度坐标,事件描述) values(?,?,?,to_date(?,'yyyy-MM-dd hh24:mi:ss'),to_date(?,'yyyy-MM-dd hh24:mi:ss'),?,?,?,?)";
                ps = connection.prepareStatement(sql1);
                ps.setString(1,uuid);
                ps.setString(2,water_body_id);
                ps.setString(3,eventNm);
                ps.setString(4,startTime);
                ps.setString(5,endTime);
                ps.setString(6,place);
                ps.setString(7,side);
                ps.setString(8,longitude+","+latitude);
                ps.setString(9, describe);
                ps.execute();
                ps.close();

                if(pollutedInfo != null){
                    String sql2 = "insert into tfsj_biz_水污染物(SN_ID,污染事件_ID,污染物序号,污染物_ID,污染物排放量) values(?,?,?,?,?)";
                    for(int i = 0 ; i < pollutedInfo.size() ; i++){
                        String snid = Md5Util.getUUID();
                        Map<String,String> waterPollution = pollutedInfo.get(i);
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1,snid);
                        ps.setString(2,uuid);
                        ps.setInt(3, i + 1);
                        ps.setString(4, waterPollution.get("mx_internal_uid"));
                        ps.setString(5,waterPollution.get("num"));
                        ps.execute();
                    }
                }

            }else{
                //修改
                String sql1 = "update tfsj_biz_水污染事件 set WATER_BODY_ID = ?," +
                        "污染事件名称 = ?," +
                        "开始时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss')," +
                        "结束时间 = to_date(?,'yyyy-MM-dd hh24:mi:ss')," +
                        "发生地点 = ?,位置_ID = ?,经纬度坐标 = ?,事件描述 = ? where 污染事件_ID = ?";
                ps = connection.prepareStatement(sql1);
                ps.setString(1,water_body_id);
                ps.setString(2,eventNm);
                ps.setString(3,startTime);
                ps.setString(4,endTime);
                ps.setString(5,place);
                ps.setString(6,side);
                ps.setString(7,longitude+","+latitude);
                ps.setString(8,describe);
                ps.setString(9,wrwEventId);
                ps.execute();
                ps.close();
                if(pollutedInfo != null){
                    String sql = "select * from tfsj_biz_水污染物 where 污染事件_ID = ?";
                    ps = connection.prepareStatement(sql);
                    ps.setString(1, wrwEventId);
                    rs = ps.executeQuery();
                    if (rs.next()){
                        //先全部进行删除
                        String sql2 = "delete from  tfsj_biz_水污染物 where 污染事件_ID = ?";
                        ps = connection.prepareStatement(sql2);
                        ps.setString(1,wrwEventId);
                        ps.execute();
                        ps.close();
                    }

                    //然后再进行insert插入数据
                    String sql3 = "insert into tfsj_biz_水污染物(SN_ID,污染事件_ID,污染物序号,污染物_ID,污染物排放量) values(?,?,?,?,?)";
                    for(int i = 0 ; i < pollutedInfo.size() ; i++){
                        String snid = Md5Util.getUUID();
                        Map<String,String> waterPollution = pollutedInfo.get(i);
                        ps = connection.prepareStatement(sql3);
                        ps.setString(1,snid);
                        ps.setString(2,wrwEventId);
                        ps.setInt(3, i + 1);
                        ps.setString(4, waterPollution.get("mx_internal_uid"));
                        ps.setString(5,waterPollution.get("num"));
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
            if(connection != null){
                connection.close();
            }
            if(ps != null){
                ps.close();
            }
            if(rs != null){
                rs.close();
            }
        }
    }

    @Override
    public void deleteWaterPollutionEvent(String pollutionEventId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from tfsj_biz_水污染事件 where 污染事件_ID = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1, pollutionEventId);
            ps.execute();
            ps.close();

            String sql2 = "select * from tfsj_biz_水污染物 where 污染事件_ID = ?";
            ps = connection.prepareStatement(sql2);
            ps.setString(1,pollutionEventId);
            rs = ps.executeQuery();
            if(rs.next()){
                String sql3 = "delete from tfsj_biz_水污染物 where 污染事件_ID = ?";
                ps = connection.prepareStatement(sql3);
                ps.setString(1,pollutionEventId);
                ps.execute();
            }
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
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
    }

    @Override
    public Map<String, Object> queryWaterPollutionEvent(String pollutionEventId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select * from tfsj_biz_水污染事件 where 污染事件_ID = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,pollutionEventId);
            rs = ps.executeQuery();
            while (rs.next()){
                resultMap.put("pollutionEventId", rs.getString(1));
                resultMap.put("waterBodyId", rs.getString(2));
                resultMap.put("pollutionEventName", rs.getString(3));
                resultMap.put("startTime", sdf.format(new Date(rs.getTimestamp(4).getTime())));
                resultMap.put("endTime", sdf.format(new Date(rs.getTimestamp(5).getTime())));
                resultMap.put("place", rs.getString(6));
                resultMap.put("side", rs.getString(7));
                resultMap.put("coordinate", rs.getString(8));
                resultMap.put("describe", rs.getString(9));
            }
            String sql2 = "select * from tfsj_biz_水污染物 where 污染事件_ID = ? order by 污染物序号";
            ps = connection.prepareStatement(sql2);
            ps.setString(1,pollutionEventId);
            rs = ps.executeQuery();
            List<Map<String,String>> list = new ArrayList<Map<String, String>>();
            while (rs.next()){
                Map<String,String> map = new LinkedHashMap<String,String>();
                map.put("pollutionID",rs.getString(4));
                map.put("emissions",rs.getString(5));
                list.add(map);
            }
            resultMap.put("wrwInfo",list);
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
}
