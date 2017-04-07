package bitc.sz.wzEnv.tfsj;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.JsonUtil;
import bitc.sz.wzEnv.util.Md5Util;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.lang.StringUtils;
import org.apache.tomcat.dbcp.dbcp.BasicDataSource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.*;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by Administrator on 2016/8/30.
 */
@Service
public class TfsjServiceImpl implements TfsjService{
    @Autowired
    private BasicDataSource basicDataSource;


    @Override
    public ExtjsResponse newWrwProperty(String number, String itemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String itemId = Md5Util.getSalt();
            String sql = "insert into tfsj_sys_污染物属性表(ITEM_ID,序号,ITEM_NAME) values(?,?,?)";
            ps = connection.prepareStatement(sql);
            ps.setString(1,itemId);
            ps.setString(2,number);
            ps.setString(3, itemName);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse updateWrwItemNameProperty(String itemId, String number, String itemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update tfsj_sys_污染物属性表 set ITEM_NAME = ? where ITEM_ID = ? and 序号 = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,itemName);
            ps.setString(2, itemId);
            ps.setString(3,number);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse updateWrwProperty(String itemId, String number, String itemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update tfsj_sys_污染物属性表 set 序号 = ?,ITEM_NAME = ? where ITEM_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,number);
            ps.setString(2,itemName);
            ps.setString(3, itemId);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public Map<String, Object> getWrwPropertyMessage() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> map = new HashMap<String,Object>();
        List<String> numberList = new ArrayList<String>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 序号 from tfsj_sys_污染物属性表";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                numberList.add(rs.getString(1));
            }
            map.put("rowCount", numberList.size());
            map.put("numberList", numberList);
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
        return map;
    }

    @Override
    public Map<String,Object> queryWrwMessage(String keyword,PagingInfo info) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        Map<String,Object> result = new HashedMap();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = " select 污染物_ID from tfsj_sys_污染物字典表";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            Set<String> wrwIdSet = new HashSet<String>();
            while (rs.next()){
                wrwIdSet.add(rs.getString(1));
            }
            String sql2 = " select t2.污染物_id, t1.item_name, t2.item_values\n" +
                    "   from (select *\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tfsj_sys_污染物属性表 order by 序号) t\n" +
                    "         where rownum < 8)) t1\n" +
                    "   left join tfsj_sys_污染物字典表 t2\n" +
                    "     on t1.item_id = t2.item_id\n" +
                    "    and t2.污染物_id = ?\n";
            Iterator wrwIds = wrwIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            while (wrwIds.hasNext()){
                String currentWrwId = (String) wrwIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1, currentWrwId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("wrwId", currentWrwId);
                while (rs.next()){
                    propertyNameSet.add(rs.getString(2));
                    resultMap.put(rs.getString(2),rs.getString(3));
                }
                tempList.add(resultMap);
            }
            if(StringUtils.isNotEmpty(keyword)){
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
            if(info.getStart() == 0 && info.getLimit() == 0){
                result.put("size",resultList.size());
                result.put("result",resultList);
                return result;
            }
            if(resultList.size()>info.getStart() && resultList.size()>info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(), info.getStart() + info.getLimit());
            }else if(resultList.size()>info.getStart() && resultList.size()<= info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(),resultList.size());
            }
            result.put("size",resultList.size());
            result.put("result",list);
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
        return result;
    }

    @Override
    public Map<String, Object> getwrwQueryGridColums() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> columnsList = new ArrayList<Map<String, Object>>();
        List<String> fieldList = new ArrayList<String>();

        Map<String,Object> resultMap= new HashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select g.item_name\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tfsj_sys_污染物属性表 order by 序号) t\n" +
                    "         where rownum < 8) g";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> columnMap = new LinkedHashMap<String,Object>();
                columnMap.put("text",rs.getString(1));
                columnMap.put("dataIndex", rs.getString(1));
                columnMap.put("flex", 1);
                columnsList.add(columnMap);
                fieldList.add(rs.getString(1));
            }

            resultMap.put("column", columnsList);
            resultMap.put("field", fieldList);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if (rs != null){
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

    @Override
    public List<Map<String, String>> queryWrwDictionaryMessage(String wrwId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t2.污染物_id, t1.item_id,t1.item_name, t2.item_values\n" +
                    "  from tfsj_sys_污染物属性表 t1\n" +
                    "  left join tfsj_sys_污染物字典表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and t2.污染物_id = ?\n" +
                    " order by t1.序号\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, wrwId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("wrwId",rs.getString(1));
                resultMap.put("item_id",rs.getString(2));
                resultMap.put("item_name",rs.getString(3));
                resultMap.put("item_values",rs.getString(4));
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
    public ExtjsResponse saveWrwDictionary(String wrwId , String editWrwData, String addWrwData) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            List<Map<String,String>> addList = null;
            List<Map<String,String>> editList = null;
            if(!"".equals(addWrwData)){
                addList = JsonUtil.readJson(addWrwData, ArrayList.class, Map.class);
            }
            if(!"".equals(editWrwData)){
                editList = JsonUtil.readJson(editWrwData, ArrayList.class, Map.class);
            }
            if("".equals(wrwId)){
                String sql1 = "insert into tfsj_sys_污染物字典表 (污染物_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                ps = connection.prepareStatement(sql1);
                String uuid = Md5Util.getSalt();
                for(int i = 0 ; i < addList.size() ; i++){
                    ps.setString(1,uuid);
                    ps.setString(2,addList.get(i).get("item_id"));
                    ps.setString(3,addList.get(i).get("item_values"));
                    ps.execute();
                }
            }else{
                if(addList != null){
                    String sql1 = "insert into tfsj_sys_污染物字典表 (污染物_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                    ps = connection.prepareStatement(sql1);
                    for(int i = 0 ; i < addList.size() ; i++){
                        ps.setString(1,addList.get(i).get("wrwId"));
                        ps.setString(2,addList.get(i).get("item_id"));
                        ps.setString(3,addList.get(i).get("item_values"));
                        ps.execute();
                    }
                }
                if(editList != null){
                    String sql2 = "update tfsj_sys_污染物字典表 set ITEM_VALUES = ? where 污染物_ID = ? and ITEM_ID = ?";
                    ps = connection.prepareStatement(sql2);
                    for(int i = 0 ; i < editList.size() ; i++){
                        ps.setString(1,editList.get(i).get("item_values"));
                        ps.setString(2,editList.get(i).get("wrwId"));
                        ps.setString(3,editList.get(i).get("item_id"));
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
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse deleteWrwDictionary(String wrwId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from tfsj_sys_污染物字典表 where 污染物_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,wrwId);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public List<Map<String, String>> queryFxyDictionaryMessage(String fxyId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t2.风险源_ID, t1.item_id,t1.item_name, t2.item_values\n" +
                    "  from tfsj_sys_风险源属性表 t1\n" +
                    "  left join tfsj_sys_风险源字典表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and t2.风险源_ID = ?\n" +
                    " order by t1.序号\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,fxyId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("fxyId",rs.getString(1));
                resultMap.put("item_id",rs.getString(2));
                resultMap.put("item_name",rs.getString(3));
                resultMap.put("item_values",rs.getString(4));
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
    public ExtjsResponse newFxyProperty(String number, String itemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String itemId = Md5Util.getSalt();
            String sql = "insert into tfsj_sys_风险源属性表(ITEM_ID,序号,ITEM_NAME) values(?,?,?)";
            ps = connection.prepareStatement(sql);
            ps.setString(1,itemId);
            ps.setString(2,number);
            ps.setString(3, itemName);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse updateFxyProperty(String itemId, String startNumber, String currentNumber, String currentItemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update tfsj_sys_风险源属性表 set 序号 = ?,ITEM_NAME = ? where ITEM_ID = ? and 序号 = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,currentNumber);
            ps.setString(2,currentItemName);
            ps.setString(3, itemId);
            ps.setString(4, startNumber);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public Map<String, Object> getFxyPropertyMessage() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> map = new HashMap<String,Object>();
        List<String> numberList = new ArrayList<String>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 序号 from tfsj_sys_风险源属性表";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                numberList.add(rs.getString(1));
            }
            map.put("rowCount", numberList.size());
            map.put("numberList", numberList);
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
        return map;
    }

    @Override
    public Map<String, Object> getfxyQueryGridColums() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> columnsList = new ArrayList<Map<String, Object>>();
        List<String> fieldList = new ArrayList<String>();

        Map<String,Object> resultMap= new HashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select g.item_name\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tfsj_sys_风险源属性表 order by 序号) t\n" +
                    "         where rownum < 8) g";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> columnMap = new LinkedHashMap<String,Object>();
                columnMap.put("text",rs.getString(1));
                columnMap.put("dataIndex",rs.getString(1));
                columnMap.put("flex", 1);
                columnsList.add(columnMap);
                fieldList.add(rs.getString(1));
            }

            resultMap.put("column", columnsList);
            resultMap.put("field", fieldList);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if (rs != null){
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

    @Override
    public Map<String,Object> queryFxyMessage(String keyword,PagingInfo info) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        Map<String,Object> result = new HashedMap();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = " select 风险源_ID from tfsj_sys_风险源字典表";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            Set<String> fxyIdSet = new HashSet<String>();
            while (rs.next()){
                fxyIdSet.add(rs.getString(1));
            }
            String sql2 = " select t2.风险源_id, t1.item_name, t2.item_values\n" +
                    "   from (select *\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tfsj_sys_风险源属性表 order by 序号) t\n" +
                    "         where rownum < 8)) t1\n" +
                    "   left join tfsj_sys_风险源字典表 t2\n" +
                    "     on t1.item_id = t2.item_id\n" +
                    "    and t2.风险源_id = ?\n";
            Iterator fxyIds = fxyIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            while (fxyIds.hasNext()){
                String currentFxyId = (String) fxyIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1,currentFxyId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("fxyId",currentFxyId);
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
                String sql4 = "select t2.obj_name\n" +
                        "  from tfsj_rel_应急信息水体关系表 t1, V_用户对应水体 t2\n" +
                        " where t1.water_body_id = t2.obj_id\n" +
                        "   and t1.tfsj_item_id = ?\n";
                ps = connection.prepareStatement(sql4);
                ps.setString(1,currentFxyId);
                rs = ps.executeQuery();
                StringBuilder waterBodyName = new StringBuilder();
                while (rs.next()){
                    waterBodyName.append(rs.getString(1)+",");
                }
                resultMap.put("适用水体",StringUtils.isEmpty(waterBodyName.toString())?"":waterBodyName.toString().substring(0,waterBodyName.toString().lastIndexOf(",")));
                tempList.add(resultMap);
            }
            if(StringUtils.isNotEmpty(keyword)){
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
            if(info.getStart() == 0 && info.getLimit() == 0){
                result.put("size",resultList.size());
                result.put("result",resultList);
                return result;
            }
            if(resultList.size()>info.getStart() && resultList.size()>info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(), info.getStart() + info.getLimit());
            }else if(resultList.size()>info.getStart() && resultList.size()<= info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(),resultList.size());
            }
            result.put("size",resultList.size());
            result.put("result",list);
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
        return result;
    }

    @Override
    public ExtjsResponse saveFxyDictionary(String fxyId, String coordinate, String editFxyData, String addFxyData) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            List<Map<String,String>> addList = null;
            List<Map<String,String>> editList = null;
            if(!"".equals(addFxyData)){
                addList = JsonUtil.readJson(addFxyData, ArrayList.class, Map.class);
            }
            if(!"".equals(editFxyData)){
                editList = JsonUtil.readJson(editFxyData, ArrayList.class, Map.class);
            }
            if("".equals(fxyId)){
                String sql1 = "insert into tfsj_sys_风险源字典表 (风险源_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                ps = connection.prepareStatement(sql1);
                String uuid = Md5Util.getSalt();
                for(int i = 0 ; i < addList.size() ; i++){
                    ps.setString(1,uuid);
                    ps.setString(2,addList.get(i).get("item_id"));
                    ps.setString(3,addList.get(i).get("item_values"));
                    ps.execute();
                }

                String sql = "insert into tfsj_sys_风险源坐标 (风险源_ID,经纬度坐标) values(?,?)";
                ps = connection.prepareStatement(sql);
                ps.setString(1,uuid);
                ps.setString(2,coordinate);
                ps.execute();
            }else{
                String sql = "update tfsj_sys_风险源坐标 set 经纬度坐标 = ? where 风险源_ID = ?";
                ps = connection.prepareStatement(sql);
                ps.setString(1,coordinate);
                ps.setString(2,fxyId);
                ps.execute();
                if(addList != null){
                    String sql1 = "insert into tfsj_sys_风险源字典表 (风险源_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                    ps = connection.prepareStatement(sql1);
                    for(int i = 0 ; i < addList.size() ; i++){
                        ps.setString(1,addList.get(i).get("fxyId"));
                        ps.setString(2,addList.get(i).get("item_id"));
                        ps.setString(3,addList.get(i).get("item_values"));
                        ps.execute();
                    }
                }
                if(editList != null){
                    String sql2 = "update tfsj_sys_风险源字典表 set ITEM_VALUES = ? where 风险源_ID = ? and ITEM_ID = ?";
                    ps = connection.prepareStatement(sql2);
                    for(int i = 0 ; i < editList.size() ; i++){
                        ps.setString(1,editList.get(i).get("item_values"));
                        ps.setString(2,editList.get(i).get("fxyId"));
                        ps.setString(3,editList.get(i).get("item_id"));
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
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse deleteFxyDictionary(String fxyId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from tfsj_sys_风险源字典表 where 风险源_ID = ?";
            String sql2 = "delete from tfsj_sys_风险源坐标 where 风险源_ID = ?";
            String sql3 = "delete from tfsj_rel_应急信息水体关系表 where TFSJ_ITEM_ID = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,fxyId);
            ps.execute();

            ps = connection.prepareStatement(sql2);
            ps.setString(1,fxyId);
            ps.execute();

            ps = connection.prepareStatement(sql3);
            ps.setString(1,fxyId);
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
        return new ExtjsResponse(true);
    }

    @Override
    public List<String> getRelatedWaterBody(String id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<String> resultList = new ArrayList<String>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t.water_body_id\n" +
                    "  from tfsj_rel_应急信息水体关系表 t\n" +
                    " where t.tfsj_item_id = ?\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,id);
            rs = ps.executeQuery();
            while (rs.next()){
                resultList.add(rs.getString(1));
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
    public void saveRelatedWaterBody(String id, String waterBodyIds) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from tfsj_rel_应急信息水体关系表 where TFSJ_ITEM_ID = ?";
            String sql2 = "insert into tfsj_rel_应急信息水体关系表 (TFSJ_ITEM_ID,WATER_BODY_ID) values(?,?)";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,id);
            ps.execute();

            ps = connection.prepareStatement(sql2);
            String[] waterBodyId = waterBodyIds.split(",");
            for(int i = 0 ; i < waterBodyId.length ; i++){
                ps.setString(1,id);
                ps.setString(2,waterBodyId[i]);
                ps.execute();
            }
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        }finally {
            if (ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public List<Map<String, String>> queryZjDictionaryMessage(String zjId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t2.专家_ID, t1.item_id,t1.item_name, t2.item_values\n" +
                    "  from tsfj_sys_专家库属性表 t1\n" +
                    "  left join tfsj_sys_专家库表 t2\n" +
                    "    on t1.item_id = t2.item_id\n" +
                    "   and t2.专家_ID = ?\n" +
                    " order by t1.序号\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1, zjId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("zjId",rs.getString(1));
                resultMap.put("item_id",rs.getString(2));
                resultMap.put("item_name",rs.getString(3));
                resultMap.put("item_values",rs.getString(4));
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
    public ExtjsResponse newZjProperty(String number, String itemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String itemId = Md5Util.getSalt();
            String sql = "insert into tsfj_sys_专家库属性表(ITEM_ID,序号,ITEM_NAME) values(?,?,?)";
            ps = connection.prepareStatement(sql);
            ps.setString(1,itemId);
            ps.setString(2,number);
            ps.setString(3, itemName);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public ExtjsResponse updateZjProperty(String itemId, String startNumber, String currentNumber, String currentItemName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "update tsfj_sys_专家库属性表 set 序号 = ?,ITEM_NAME = ? where ITEM_ID = ? and 序号 = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,currentNumber);
            ps.setString(2,currentItemName);
            ps.setString(3, itemId);
            ps.setString(4, startNumber);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            ps.close();
            connection.close();
        }
        return new ExtjsResponse(true);
    }

    @Override
    public Map<String, Object> getZjPropertyMessage() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> map = new HashMap<String,Object>();
        List<String> numberList = new ArrayList<String>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 序号 from tsfj_sys_专家库属性表";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                numberList.add(rs.getString(1));
            }
            map.put("rowCount", numberList.size());
            map.put("numberList", numberList);
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
        return map;
    }

    @Override
    public Map<String, Object> getzjQueryGridColums() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> columnsList = new ArrayList<Map<String, Object>>();
        List<String> fieldList = new ArrayList<String>();

        Map<String,Object> resultMap= new HashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select g.item_name\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tsfj_sys_专家库属性表 order by 序号) t\n" +
                    "         where rownum < 8) g";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> columnMap = new LinkedHashMap<String,Object>();
                columnMap.put("text",rs.getString(1));
                columnMap.put("dataIndex", rs.getString(1));
                columnMap.put("flex",1);
                columnsList.add(columnMap);
                fieldList.add(rs.getString(1));
            }

            resultMap.put("column", columnsList);
            resultMap.put("field", fieldList);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if (rs != null){
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

    @Override
    public Map<String,Object> queryZjMessage(String keyword,PagingInfo info) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> resultList = new ArrayList<Map<String, String>>();
        List<Map<String,String>> tempList = new ArrayList<Map<String, String>>();
        //存放结果
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        Map<String,Object> result = new HashedMap();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = " select 专家_ID from tfsj_sys_专家库表";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            Set<String> zjIdSet = new HashSet<String>();
            while (rs.next()){
                zjIdSet.add(rs.getString(1));
            }
            String sql2 = " select t2.专家_ID, t1.item_name, t2.item_values\n" +
                    "   from (select *\n" +
                    "  from (select t.*, rownum rownum_\n" +
                    "          from (select * from tsfj_sys_专家库属性表 order by 序号) t\n" +
                    "         where rownum < 8)) t1\n" +
                    "   left join tfsj_sys_专家库表 t2\n" +
                    "     on t1.item_id = t2.item_id\n" +
                    "    and t2.专家_ID = ?\n";
            Iterator zjIds = zjIdSet.iterator();
            Set<String> propertyNameSet = new HashSet<String>();
            while (zjIds.hasNext()){
                String currentZjId = (String) zjIds.next();
                ps = connection.prepareStatement(sql2);
                ps.setString(1, currentZjId);
                rs = ps.executeQuery();
                Map<String,String> resultMap = new HashMap<String,String>();
                resultMap.put("zjId", currentZjId);
                while (rs.next()){
                    propertyNameSet.add(rs.getString(2));
                    resultMap.put(rs.getString(2),rs.getString(3));
                }
                String sql3 = "select 上传文件名 from tfsj_sys_专家照片 where 专家_ID = ?";
                ps = connection.prepareStatement(sql3);
                ps.setString(1, currentZjId);
                rs = ps.executeQuery();
                if (rs.next()){
                    resultMap.put("上传文件名",rs.getString(1));
                }
                String sql4 = "select t2.obj_name\n" +
                        "  from tfsj_rel_应急信息水体关系表 t1, V_用户对应水体 t2\n" +
                        " where t1.water_body_id = t2.obj_id\n" +
                        "   and t1.tfsj_item_id = ?\n";
                ps = connection.prepareStatement(sql4);
                ps.setString(1,currentZjId);
                rs = ps.executeQuery();
                StringBuilder waterBodyName = new StringBuilder();
                while (rs.next()){
                    waterBodyName.append(rs.getString(1)+",");
                }
                resultMap.put("适用水体",StringUtils.isEmpty(waterBodyName.toString())?"":waterBodyName.toString().substring(0,waterBodyName.toString().lastIndexOf(",")));
                tempList.add(resultMap);
            }
            if(StringUtils.isNotEmpty(keyword)){
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
            if(info.getStart() == 0 && info.getLimit() == 0){
                result.put("size",resultList.size());
                result.put("result",resultList);
                return result;
            }
            if(resultList.size()>info.getStart() && resultList.size()>info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(), info.getStart() + info.getLimit());
            }else if(resultList.size()>info.getStart() && resultList.size()<= info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(),resultList.size());
            }
            result.put("size",resultList.size());
            result.put("result",list);
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
        return result;
    }

    @Override
    public void saveZjDictionary(String zjId, String editZjData, String addZjData,MultipartFile photo) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            List<Map<String,String>> addList = null;
            List<Map<String,String>> editList = null;
            if(!"".equals(addZjData)){
                addList = JsonUtil.readJson(addZjData, ArrayList.class, Map.class);
            }
            if(!"".equals(editZjData)){
                editList = JsonUtil.readJson(editZjData, ArrayList.class, Map.class);
            }
            if("".equals(zjId)){
                String sql1 = "insert into tfsj_sys_专家库表 (专家_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                ps = connection.prepareStatement(sql1);
                String uuid = Md5Util.getSalt();
                for(int i = 0 ; i < addList.size() ; i++){
                    ps.setString(1,uuid);
                    ps.setString(2,addList.get(i).get("item_id"));
                    ps.setString(3,addList.get(i).get("item_values"));
                    ps.execute();
                }

                String sql = "insert into tfsj_sys_专家照片 (专家_ID,照片,上传文件名) values(?,?,?)";
                ps = connection.prepareStatement(sql);
                ps.setString(1, uuid);
                ps.setBytes(2, photo.getBytes());
                ps.setString(3,photo.getOriginalFilename());
                ps.execute();
            }else{
                if(!photo.isEmpty()){
                    String sql = "update tfsj_sys_专家照片 set 照片 = ?,上传文件名 = ? where 专家_ID = ?";
                    ps = connection.prepareStatement(sql);
                    ps.setBytes(1, photo.getBytes());
                    ps.setString(2,photo.getOriginalFilename());
                    ps.setString(3,zjId);
                    ps.execute();
                }
                if(addList != null){
                    String sql1 = "insert into tfsj_sys_专家库表 (专家_ID,ITEM_ID,ITEM_VALUES) values(?,?,?)";
                    ps = connection.prepareStatement(sql1);
                    for(int i = 0 ; i < addList.size() ; i++){
                        ps.setString(1,addList.get(i).get("zjId"));
                        ps.setString(2,addList.get(i).get("item_id"));
                        ps.setString(3,addList.get(i).get("item_values"));
                        ps.execute();
                    }
                }
                if(editList != null){
                    String sql2 = "update tfsj_sys_专家库表 set ITEM_VALUES = ? where 专家_ID = ? and ITEM_ID = ?";
                    ps = connection.prepareStatement(sql2);
                    for(int i = 0 ; i < editList.size() ; i++){
                        ps.setString(1,editList.get(i).get("item_values"));
                        ps.setString(2,editList.get(i).get("zjId"));
                        ps.setString(3,editList.get(i).get("item_id"));
                        ps.execute();
                    }
                }
            }
            connection.commit();
        } catch (SQLException e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public BufferedInputStream getZjPhoto(String zjId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Blob blob = null;
        BufferedInputStream bufferedInputStream = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 照片 from tfsj_sys_专家照片 where 专家_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1, zjId);
            rs = ps.executeQuery();
            if (rs.next()) {
                blob = rs.getBlob(1);
            }
            if(blob != null){
                InputStream in = blob.getBinaryStream();
                bufferedInputStream = new BufferedInputStream(in);
            }

            return bufferedInputStream;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    @Override
    public ExtjsResponse deleteZjDictionary(String zjId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from tfsj_sys_专家库表 where 专家_ID = ?";
            String sql2 = "delete from tfsj_sys_专家照片 where 专家_ID = ?";
            String sql3 = "delete from tfsj_rel_应急信息水体关系表 where TFSJ_ITEM_ID = ?";

            ps = connection.prepareStatement(sql1);
            ps.setString(1,zjId);
            ps.execute();

            ps = connection.prepareStatement(sql2);
            ps.setString(1,zjId);
            ps.execute();

            ps = connection.prepareStatement(sql3);
            ps.setString(1,zjId);
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
        return new ExtjsResponse(true);
    }

    @Override
    public Map<String,Object> queryPlanAndLawMessage(String keyword, PagingInfo info,String typeId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps1 = null;
        ResultSet rs1 = null;
        PreparedStatement ps2 = null;
        ResultSet rs2 = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Map<String,Object> result = new HashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t.应急预案_id, t.应急预案名称, t.编制日期, t.type_id, t.上传文件名\n" +
                    "  from tfsj_biz_应急预案法规表 t\n" +
                    " where t.type_id = ?\n";
            String sql2 = "select t2.obj_name\n" +
                    "  from tfsj_rel_应急信息水体关系表 t1, v_用户对应水体 t2\n" +
                    " where t1.water_body_id = t2.obj_id\n" +
                    "   and t1.tfsj_item_id = ?";
            ps1 = connection.prepareStatement(sql1);
            ps2 = connection.prepareStatement(sql2);
            ps1.setString(1,typeId);
            rs1 = ps1.executeQuery();
            List<Map<String,String>> tempList = new ArrayList<Map<String,String>>();
            while (rs1.next()){
                Map<String,String> tempMap = new LinkedHashMap<String,String>();
                tempMap.put("planId",rs1.getString(1));
                tempMap.put("planName",rs1.getString(2));
                tempMap.put("date",sdf.format(rs1.getDate(3)));
                tempMap.put("typeId",rs1.getString(4));
                tempMap.put("fileName",rs1.getString(5));
                ps2.setString(1,rs1.getString(1));
                rs2 = ps2.executeQuery();
                StringBuilder waterBodyName = new StringBuilder();
                while (rs2.next()){
                    waterBodyName.append(rs2.getString(1)+",");
                }
                tempMap.put("relateWaterBody",StringUtils.isEmpty(waterBodyName.toString())?"":waterBodyName.toString().substring(0,waterBodyName.toString().lastIndexOf(",")));
                tempList.add(tempMap);
            }
            List<Map<String,String>> resultList = new ArrayList<Map<String,String>>();
            if(StringUtils.isNotEmpty(keyword)){
                for (int i = 0 ; i < tempList.size() ; i++){
                    Map<String,String> map = tempList.get(i);
                    if(map.get("planName").contains(keyword)){
                        resultList.add(map);
                    }
                }
            }else{
                resultList.addAll(tempList);
            }
            if(info.getStart() == 0 && info.getLimit() == 0){
                result.put("size",resultList.size());
                result.put("result",resultList);
                return result;
            }
            List<Map<String,String>> list = new ArrayList<Map<String,String>>();
            if(resultList.size()>info.getStart() && resultList.size()>info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(), info.getStart() + info.getLimit());
            }else if(resultList.size()>info.getStart() && resultList.size()<= info.getStart()+info.getLimit()){
                list = resultList.subList(info.getStart(),resultList.size());
            }
            result.put("size",resultList.size());
            result.put("result",list);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(rs1 != null){
                rs1.close();
            }
            if(ps1 != null){
                ps1.close();
            }
            if(rs2 != null){
                rs2.close();
            }
            if(ps2 != null){
                ps2.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return result;
    }

    @Override
    public void savePlanAndLaws(String planAndLawsId, String planAndLawsName, Long planAndLawsTime, String planAndLawsType, MultipartFile file) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            if("".equals(planAndLawsId)){
                String uuid = Md5Util.getUUID();
                String sql = "insert into tfsj_biz_应急预案法规表 (应急预案_ID,应急预案名称,编制日期,TYPE_ID,附件_OBJ,上传文件名) values(?,?,?,?,?,?)";
                ps = connection.prepareStatement(sql);
                ps.setString(1,uuid);
                ps.setString(2,planAndLawsName);
                ps.setDate(3, new Date(planAndLawsTime));
                ps.setString(4, planAndLawsType);
                ps.setBytes(5, file.getBytes());
                ps.setString(6,file.getOriginalFilename());
                ps.execute();
            }else{
                if(!file.isEmpty()){
                    String sql = "update tfsj_biz_应急预案法规表 set 应急预案名称 = ?,编制日期 = to_date(?,'yyyy-MM-dd'),附件_OBJ = ? ,上传文件名 = ? where 应急预案_ID = ?";
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                    String date =sdf.format(new java.util.Date(planAndLawsTime));
                    ps = connection.prepareStatement(sql);
                    ps.setString(1,planAndLawsName);
                    ps.setString(2,date);
                    ps.setBytes(3, file.getBytes());
                    ps.setString(4,file.getOriginalFilename());
                    ps.setString(5, planAndLawsId);
                    ps.execute();
                }else{
                    String sql = "update tfsj_biz_应急预案法规表 set 应急预案名称 = ?,编制日期 = to_date(?,'yyyy-MM-dd') where 应急预案_ID = ?";
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                    String date =sdf.format(new java.util.Date(planAndLawsTime));
                    ps = connection.prepareStatement(sql);
                    ps.setString(1,planAndLawsName);
                    ps.setString(2, date);
                    ps.setString(3, planAndLawsId);
                    ps.execute();
                }

            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public ExtjsResponse deletePlanAndLaws(String planAndLawsId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            connection.setAutoCommit(false);
            String sql1 = "delete from tfsj_biz_应急预案法规表 where 应急预案_ID = ?";
            String sql2 = "delete from tfsj_rel_应急信息水体关系表 where TFSJ_ITEM_ID = ?";
            ps = connection.prepareStatement(sql1);
            ps.setString(1,planAndLawsId);
            ps.execute();

            ps = connection.prepareStatement(sql2);
            ps.setString(1,planAndLawsId);
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
        return new ExtjsResponse(true);
    }

    @Override
    public Map<String,Object> downLoadPlanAndLaws(String planAndLawsId) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select 附件_OBJ,上传文件名 from tfsj_biz_应急预案法规表 where 应急预案_ID = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1, planAndLawsId);
            rs = ps.executeQuery();
            if (rs.next()) {
                Blob blob = rs.getBlob(1);
                InputStream in = blob.getBinaryStream();
                BufferedInputStream inputStream = new BufferedInputStream(in);
                String fileName = rs.getString(2);
                resultMap.put("fileName",fileName);
                resultMap.put("file",inputStream);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            if (rs != null) {
                rs.close();
            }
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
        return resultMap;
    }
}
