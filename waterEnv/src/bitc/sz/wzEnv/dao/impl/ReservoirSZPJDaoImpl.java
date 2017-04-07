package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.dao.ReservoirSZPJDao;
import bitc.sz.wzEnv.dao.SuperMapDao;
import bitc.sz.wzEnv.exception.NullException;
import bitc.sz.wzEnv.util.MapSortUtil;
import oracle.jdbc.OracleTypes;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import javax.sql.DataSource;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Administrator on 2016/7/13.
 */
@Repository
public class ReservoirSZPJDaoImpl implements ReservoirSZPJDao {
    @Autowired
    private DataSource basicDataSource;

    @Override
    public List<Map<String, String>> getNutritionItems(String water_body_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List<Map<String,String>> itemsList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t.item_id, t.sys_name\n" +
                    "  from szpj_sys_指标项 t\n" +
                    " where t.domain_id = '2'\n" +
                    "   and t.itype_id = '1'\n" +
                    " order by t.sys_id\n";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            ResultSetMetaData metaData = rs.getMetaData();
            while (rs.next()){
                Map map = new LinkedHashMap();
                for(int i = 0 ; i < metaData.getColumnCount(); i++){
                    String columnName = metaData.getColumnName(i+1);
                    map.put(columnName,rs.getString(columnName));
                }
                itemsList.add(map);
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
        return itemsList;
    }

    @Override
    public List<Map<Object, Object>> getReservoirSZPJYearList(String water_body_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Object, Object>> resultList = new ArrayList<Map<Object,Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select t.obj_id\n" +
                    "  from v_水资源对象 t\n" +
                    " where t.map_id = 'yuqiao'\n" +
                    "   and t.level_2_id in ('WQ', 'RR')\n" +
                    "union\n" +
                    "select t.obj_id\n" +
                    "  from v_水资源对象 t\n" +
                    " where t.map_id = 'yuqiao'\n" +
                    "   and t.level_1_id = '8'\n";
            ps = connection.prepareStatement(sql1);
            rs = ps.executeQuery();
            List<String> idList = new ArrayList<String>();
            while (rs.next()){
                idList.add(rs.getString(1));
            }
            ps = null;
            rs = null;
            String sql2 = "select distinct to_char(t1.观测时间, 'yyyy') year\n" +
                    "  from szpj_biz_水质指标观测数据 t1, szpj_sys_指标项 t2\n" +
                    " where t1.观测item_id = t2.item_id\n" +
                    "   and t1.观测点_id = ?\n" +
                    "   and t2.item_id in ('57D10B75A4F544C78168F625A4027347',\n" +
                    "                      '363DFE51C5C948C6BF59E15DE3C8BE01',\n" +
                    "                      'C393BD2BD2F6486EA0858FDD8DA69C85',\n" +
                    "                      '4637E7516E674B728334EE73B4F40021',\n" +
                    "                      'E6D46CB433014055A10FDC63149D67E5')\n" +
                    " order by year";
            ps = connection.prepareStatement(sql2);
            Set<Integer> yearSet = new HashSet<Integer>();
            for(int i = 0 ; i < idList.size() ; i++){
                ps.setString(1,idList.get(i));
                rs = ps.executeQuery();
                while (rs.next()){
                    yearSet.add(rs.getInt(1));
                }
            }
            Iterator year = yearSet.iterator();
            while (year.hasNext()){
                Map<Object,Object> resultMap = new LinkedHashMap<Object,Object>();
                int currentYear = (int) year.next();
                resultMap.put("year",currentYear);
                resultList.add(resultMap);
            }
            MapSortUtil.desc(resultList,"year","int");
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

//    @Override
//    public List getSZPJItemList(Connection connection,String water_body_id, String obj_id, String period_id, String searchType, String[] itemArr) throws SQLException {
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//
//        //存放结果
//        List resultList = new ArrayList();
//        Map<String,String> dateMap = superMapDao.getStartAndEndDate(period_id,searchType,connection);
//        String startDate = dateMap.get("startDate");
//        String endDate = dateMap.get("endDate");
//        try{
//            if(itemArr.length>0){
//                String sql = "select t6.水体_id,\n" +
//                        "       t6.水体名称,\n" +
//                        "       t1.观测点_id,\n" +
//                        "       t5.rb_nm,\n" +
//                        "       t1.观测时间,\n" +
//                        "       t1.观测item_id,\n" +
//                        "       t2.sys_name,\n" +
//                        "       t1.观测值\n" +
//                        "  from szpj_biz_水质指标观测数据   t1,\n" +
//                        "       szpj_sys_指标项             t2,\n" +
//                        "       mgmb_rel_断面与水体水功能区 t3,\n" +
//                        "       mgmb_河道断面信息简表       t4,\n" +
//                        "       wr_rb_b                     t5,\n" +
//                        "       v_水体                      t6\n" +
//                        " where t1.观测item_id = t2.item_id\n" +
//                        "   and t1.观测item_id = ?\n" +
//                        "   and to_char(t1.观测时间, 'yyyy-MM-dd') between ? and ?\n" +
//                        "   and t3.河道断面_id = t1.观测点_id\n" +
//                        "   and t3.河道断面_id = ?\n" +
//                        "   and t3.水体_id = ?\n" +
//                        "   and t4.河道断面_id = t3.河道断面_id\n" +
//                        "   and t4.wr_sd_code = t5.rb_cd\n" +
//                        "   and t3.水体_id = t6.水体_id\n";
//                ps = connection.prepareStatement(sql);
//                for (int i = 0 ; i < itemArr.length ; i++){
//                    ps.setString(1,itemArr[i]);
//                    ps.setString(2,startDate);
//                    ps.setString(3,endDate);
//                    ps.setString(4,obj_id);
//                    ps.setString(5,water_body_id);
//                    rs = ps.executeQuery();
//                    String dmId = null;
//                    String dmName = null;
//                    String itemId = null;
//                    String itemName = null;
//                    double sumValue = 0;
//                    int count = 0;
//                    while (rs.next()){
//                        dmId = rs.getString(3);
//                        dmName = rs.getString(4);
//                        itemId = rs.getString(6);
//                        itemName = rs.getString(7);
//                        sumValue = sumValue +  rs.getDouble(8);
//                        count++;
//                    }
//                    if(dmId != null){
//                        double avgValue = sumValue/count;
//                        Map resultMap = new HashMap();
//                        resultMap.put("dmId",dmId);
//                        resultMap.put("dmName",dmName);
//                        resultMap.put("itemId",itemId);
//                        resultMap.put("itemName",itemName);
//                        resultMap.put("itemValue", avgValue);
//                        if(resultMap.size()>0){
//                            resultList.add(resultMap);
//                        }
//                    }
//                }
//            }
//
//        }catch (SQLException e){
//            e.printStackTrace();
//            throw e;
//        }finally {
//            if(rs != null){
//                rs.close();
//            }
//            if(ps != null){
//                ps.close();
//            }
//        }
//        return resultList;
//    }

//    @Override
//    public Map<String, Object> getSZPJNutrition(String water_body_id, String obj_id, String period_id, String searchType, String itemIDs) throws SQLException {
//        Connection connection = null;
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//        //存放结果
//        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//        String dmId = null;
//        String dmName = null;
//        try {
//            connection = basicDataSource.getConnection();
//            if(itemIDs != null){
//                String[] itemArr = itemIDs.split(",");
//                List itemList = getSZPJItemList(connection,water_body_id,obj_id,period_id,searchType,itemArr);
//                if(itemList.size()>0){
//                    //存放中国湖泊（水库）部分参数与chla的相关关系
//                    List itemStandardList = new ArrayList();
//                    String sql2 = "select t1.指数项目名称, t2.item_id, t3.sys_name, t2.item_value\n" +
//                            "  from szpj_sys_湖泊指数项       t1,\n" +
//                            "       szpj_sys_湖泊chla关系指数 t2,\n" +
//                            "       szpj_sys_指标项           t3\n" +
//                            " where t1.sn_id = t2.富养指标_id\n" +
//                            "   and t2.item_id = t3.item_id\n" +
//                            "   and t2.item_id = ?\n";
//                    ps = connection.prepareStatement(sql2);
//                    for(int m = 0 ; m < itemArr.length ; m++){
//                        ps.setString(1,itemArr[m]);
//                        rs = ps.executeQuery();
//                        while (rs.next()){
//                            Map map1 = new HashMap();
//                            map1.put("itemID",rs.getString(2));
//                            map1.put("itemName",rs.getString(3));
//                            map1.put("itemValue",rs.getDouble(4));
//                            itemStandardList.add(map1);
//                        }
//                    }
//                    //r_ij的平方
//                    double sumStandardValue = 0;
//                    for(int n = 0 ; n < itemStandardList.size() ; n++){
//                        Map map1 = (Map) itemStandardList.get(n);
//                        double value = (double)map1.get("itemValue");
//                        sumStandardValue += value*value;
//                    }
//                    double sumValue = 0;
//                    for(int j = 0 ; j < itemList.size() ; j++){
//                        Map itemMap = (Map) itemList.get(j);
//                        String name = (String) itemMap.get("itemName");
//                        if(dmId == null){
//                            dmId = (String) itemMap.get("dmId");
//                        }
//                        if(dmName == null){
//                            dmName = (String) itemMap.get("dmName");
//                        }
//                        double value = (double)itemMap.get("itemValue");
//                        double tli = 0;
//                        double wj = 0;
//                        switch (name){
//                            case "叶绿素a(chla)" :
//                                tli = 10*(2.5+1.086*Math.log(value));
//                                break;
//                            case "总磷(TP)" :
//                                tli = 10*(9.436+1.624*Math.log(value));
//                                break;
//                            case "总氮(TN)" :
//                                tli = 10*(5.453+1.694*Math.log(value));
//                                break;
//                            case "透明度(SD)" :
//                                tli = 10*(5.118-1.94*Math.log(value));
//                                break;
//                            case "高锰酸盐指数(CODMn)" :
//                                tli = 10*(0.109+2.661*Math.log(value));
//                                break;
//                        }
//                        for(int k = 0 ; k < itemStandardList.size() ; k++){
//                            Map itemStandardMap = (Map) itemStandardList.get(k);
//                            String standardName = (String) itemStandardMap.get("itemName");
//                            double standardValue = (double)itemStandardMap.get("itemValue");
//                            if(standardName.equals(name)){
//                                switch (name){
//                                    case "叶绿素a(chla)" :
//                                        wj = standardValue*standardValue/sumStandardValue;
//                                        break;
//                                    case "总磷(TP)" :
//                                        wj = standardValue*standardValue/sumStandardValue;
//                                        break;
//                                    case "总氮(TN)" :
//                                        wj = standardValue*standardValue/sumStandardValue;
//                                        break;
//                                    case "透明度(SD)" :
//                                        wj = standardValue*standardValue/sumStandardValue;
//                                        break;
//                                    case "高锰酸盐指数(CODMn)" :
//                                        wj = standardValue*standardValue/sumStandardValue;
//                                        break;
//                                }
//                            }
//                        }
//                        sumValue = sumValue + wj*tli;
//                    }
//                    //存放营养状态等级
//                    List<Map<String,String>> gradeList = new ArrayList<Map<String, String>>();
//                    String sql3 = "select t.等级名称,t.等级颜色 from szpj_sys_湖泊营养状态表 t";
//                    ps = connection.prepareStatement(sql3);
//                    rs = ps.executeQuery();
//                    while (rs.next()){
//                        Map<String,String> gradeMap = new HashMap<String,String>();
//                        gradeMap.put("gradeName",rs.getString(1));
//                        gradeMap.put("gradeColor",rs.getString(2));
//                        gradeList.add(gradeMap);
//                    }
//                    //sumValue和szpj_sys_湖泊营养状态表相比较，确定营养状态等级以及颜色
//                    resultMap.put("dmId",dmId);
//                    resultMap.put("dmName",dmName);
//                    if(sumValue<30){
//                        resultMap.put("nutritionCondition","贫营养");
//                        for(int i = 0 ; i < gradeList.size() ; i++){
//                            if("贫营养".equals(gradeList.get(i).get("gradeName"))){
//                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
//                            }
//                        }
//                    }else if(sumValue>=30 && sumValue<=50){
//                        resultMap.put("nutritionCondition","中营养");
//                        for(int i = 0 ; i < gradeList.size() ; i++){
//                            if("中营养".equals(gradeList.get(i).get("gradeName"))){
//                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
//                            }
//                        }
//                    }else {
//                        resultMap.put("nutritionCondition","富营养");
//                        for(int i = 0 ; i < gradeList.size() ; i++){
//                            if("富营养".equals(gradeList.get(i).get("gradeName"))){
//                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
//                            }
//                        }
//                        if(sumValue>50 && sumValue<= 60){
//                            resultMap.put("nutritionCondition","轻度富营养");
//                            for(int i = 0 ; i < gradeList.size() ; i++){
//                                if("轻度富营养".equals(gradeList.get(i).get("gradeName"))){
//                                    resultMap.put("gradeColor2",gradeList.get(i).get("gradeColor"));
//                                }
//                            }
//                        }else if(sumValue > 60 && sumValue <= 70){
//                            resultMap.put("nutritionCondition","中度富营养");
//                            for(int i = 0 ; i < gradeList.size() ; i++){
//                                if("中度富营养".equals(gradeList.get(i).get("gradeName"))){
//                                    resultMap.put("gradeColor2",gradeList.get(i).get("gradeColor"));
//                                }
//                            }
//                        }else{
//                            resultMap.put("nutritionCondition","重度富营养");
//                            for(int i = 0 ; i < gradeList.size() ; i++){
//                                if("重度富营养".equals(gradeList.get(i).get("gradeName"))){
//                                    resultMap.put("gradeColor2",gradeList.get(i).get("gradeColor"));
//                                }
//                            }
//                        }
//                    }
//                }
//
//            }
//        } catch (SQLException e) {
//            e.printStackTrace();
//            throw e;
//        }finally {
//            if(rs != null){
//                rs.close();
//            }
//            if (ps != null){
//                ps.close();
//            }
//            if(connection != null){
//                connection.close();
//            }
//        }
//        return resultMap;
//    }


    public List getReservoirSZPJItemList(Connection connection,String water_body_id, String year, String[] itemArr) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        List resultList = new ArrayList();
        String startDate = year+"-01-01";
        String endDate = year+"-12-31";
        try{
            if(itemArr.length>0){
                String sql = "select t6.水体_id,\n" +
                        "       t6.水体名称,\n" +
                        "       t1.观测点_id,\n" +
                        "       t5.rb_nm,\n" +
                        "       t1.观测时间,\n" +
                        "       t1.观测item_id,\n" +
                        "       t2.sys_name,\n" +
                        "       t1.观测值\n" +
                        "  from szpj_biz_水质指标观测数据   t1,\n" +
                        "       szpj_sys_指标项             t2,\n" +
                        "       mgmb_rel_断面与水体水功能区 t3,\n" +
                        "       mgmb_河道断面信息简表       t4,\n" +
                        "       wr_rb_b                     t5,\n" +
                        "       v_水体                      t6\n" +
                        " where t1.观测item_id = t2.item_id\n" +
                        "   and t1.观测item_id = ?\n" +
                        "   and to_char(t1.观测时间, 'yyyy-MM-dd') between ? and ?\n" +
                        "   and t3.河道断面_id = t1.观测点_id      \n" +
                        "   and t3.水体_id = ?\n" +
                        "   and t4.河道断面_id = t3.河道断面_id\n" +
                        "   and t4.wr_sd_code = t5.rb_cd\n" +
                        "   and t3.水体_id = t6.水体_id";
                ps = connection.prepareStatement(sql);
                for (int i = 0 ; i < itemArr.length ; i++){
                    ps.setString(1,itemArr[i]);
                    ps.setString(2,startDate);
                    ps.setString(3,endDate);
                    ps.setString(4,water_body_id);
                    rs = ps.executeQuery();
                    String skId = null;
                    String skName = null;
                    String itemId = null;
                    String itemName = null;
                    double sumValue = 0;
                    int count = 0;
                    while (rs.next()){
                        skId = rs.getString(1);
                        skName = rs.getString(2);
                        itemId = rs.getString(6);
                        itemName = rs.getString(7);
                        sumValue = sumValue +  rs.getDouble(8);
                        count++;
                    }
                    if(skId != null){
                        double avgValue = sumValue/count;
                        Map resultMap = new HashMap();
                        resultMap.put("skId",skId);
                        resultMap.put("skName",skName);
                        resultMap.put("itemId",itemId);
                        resultMap.put("itemName",itemName);
                        resultMap.put("itemValue", avgValue);
                        if(resultMap.size()>0){
                            resultList.add(resultMap);
                        }
                    }
                }
            }

        }catch (SQLException e){
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
    public Map<String, Object> getReservoirSZPJNutrition(String water_body_id, String year, String itemIDs) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        //存放结果
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        String skId = null;
        String skName = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "select super_obj_id from v_水资源对象 where obj_id = ?";
            if(itemIDs != null){
                String[] itemArr = itemIDs.split(",");
                List itemList = getReservoirSZPJItemList(connection, water_body_id, year, itemArr);
                if(itemList.size()>0){
                    //存放中国湖泊（水库）部分参数与chla的相关关系
                    List itemStandardList = new ArrayList();
                    String sql2 = "select t1.指数项目名称, t2.item_id, t3.sys_name, t2.item_value\n" +
                            "  from szpj_sys_湖泊指数项       t1,\n" +
                            "       szpj_sys_湖泊chla关系指数 t2,\n" +
                            "       szpj_sys_指标项           t3\n" +
                            " where t1.sn_id = t2.富养指标_id\n" +
                            "   and t2.item_id = t3.item_id\n" +
                            "   and t2.item_id = ?\n";
                    ps = connection.prepareStatement(sql2);
                    for(int i = 0 ; i < itemList.size() ; i++){
                        Map itemMap = (Map) itemList.get(i);
                        String itemId = (String) itemMap.get("itemId");
                        ps.setString(1,itemId);
                        rs = ps.executeQuery();
                        while (rs.next()){
                            Map map1 = new HashMap();
                            map1.put("itemID",rs.getString(2));
                            map1.put("itemName",rs.getString(3));
                            map1.put("itemValue",rs.getDouble(4));
                            itemStandardList.add(map1);
                        }
                    }
                    //r_ij的平方
                    double sumStandardValue = 0;
                    for(int n = 0 ; n < itemStandardList.size() ; n++){
                        Map map1 = (Map) itemStandardList.get(n);
                        double value = (double)map1.get("itemValue");
                        sumStandardValue += value*value;
                    }
                    double sumValue = 0;
                    for(int j = 0 ; j < itemList.size() ; j++){
                        Map itemMap = (Map) itemList.get(j);
                        String name = (String) itemMap.get("itemName");
                        if(skId == null){
                            skId = (String) itemMap.get("skId");
                        }
                        if(skName == null){
                            skName = (String) itemMap.get("skName");
                        }
                        double value = (double)itemMap.get("itemValue");
                        double tli = 0;
                        double wj = 0;
                        switch (name){
                            case "叶绿素a(chla)" :
                                tli = 10*(2.5+1.086*Math.log(value));
                                break;
                            case "总磷(TP)" :
                                tli = 10*(9.436+1.624*Math.log(value));
                                break;
                            case "总氮(TN)" :
                                tli = 10*(5.453+1.694*Math.log(value));
                                break;
                            case "透明度(SD)" :
                                tli = 10*(5.118-1.94*Math.log(value));
                                break;
                            case "高锰酸盐指数(CODMn)" :
                                tli = 10*(0.109+2.661*Math.log(value));
                                break;
                        }
                        for(int k = 0 ; k < itemStandardList.size() ; k++){
                            Map itemStandardMap = (Map) itemStandardList.get(k);
                            String standardName = (String) itemStandardMap.get("itemName");
                            double standardValue = (double)itemStandardMap.get("itemValue");
                            if(standardName.equals(name)){
                                switch (name){
                                    case "叶绿素a(chla)" :
                                        wj = standardValue*standardValue/sumStandardValue;
                                        break;
                                    case "总磷(TP)" :
                                        wj = standardValue*standardValue/sumStandardValue;
                                        break;
                                    case "总氮(TN)" :
                                        wj = standardValue*standardValue/sumStandardValue;
                                        break;
                                    case "透明度(SD)" :
                                        wj = standardValue*standardValue/sumStandardValue;
                                        break;
                                    case "高锰酸盐指数(CODMn)" :
                                        wj = standardValue*standardValue/sumStandardValue;
                                        break;
                                }
                            }
                        }
                        sumValue = sumValue + wj*tli;
                    }
                    //存放营养状态等级
                    List<Map<String,String>> gradeList = new ArrayList<Map<String, String>>();
                    String sql3 = "select t.等级名称,t.等级颜色 from szpj_sys_湖泊营养状态表 t";
                    ps = connection.prepareStatement(sql3);
                    rs = ps.executeQuery();
                    while (rs.next()){
                        Map<String,String> gradeMap = new HashMap<String,String>();
                        gradeMap.put("gradeName",rs.getString(1));
                        gradeMap.put("gradeColor",rs.getString(2));
                        gradeList.add(gradeMap);
                    }
                    //sumValue和szpj_sys_湖泊营养状态表相比较，确定营养状态等级以及颜色
                    resultMap.put("skId",skId);
                    resultMap.put("skName",skName);
                    ps = connection.prepareStatement(sql);
                    ps.setString(1,skId);
                    rs = ps.executeQuery();
                    if(rs.next()){
                        resultMap.put("super_obj_id",rs.getString(1));
                    }
                    if(sumValue<30){
                        resultMap.put("nutritionCondition","贫营养");
                        for(int i = 0 ; i < gradeList.size() ; i++){
                            if("贫营养".equals(gradeList.get(i).get("gradeName"))){
                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
                            }
                        }
                    }else if(sumValue>=30 && sumValue<=50){
                        resultMap.put("nutritionCondition","中营养");
                        for(int i = 0 ; i < gradeList.size() ; i++){
                            if("中营养".equals(gradeList.get(i).get("gradeName"))){
                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
                            }
                        }
                    }else {
//                        resultMap.put("nutritionCondition","富营养");
//                        for(int i = 0 ; i < gradeList.size() ; i++){
//                            if("富营养".equals(gradeList.get(i).get("gradeName"))){
//                                resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
//                            }
//                        }
                        if(sumValue>50 && sumValue<= 60){
                            resultMap.put("nutritionCondition","轻度富营养");
                            for(int i = 0 ; i < gradeList.size() ; i++){
                                if("轻度富营养".equals(gradeList.get(i).get("gradeName"))){
                                    resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
                                }
                            }
                        }else if(sumValue > 60 && sumValue <= 70){
                            resultMap.put("nutritionCondition","中度富营养");
                            for(int i = 0 ; i < gradeList.size() ; i++){
                                if("中度富营养".equals(gradeList.get(i).get("gradeName"))){
                                    resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
                                }
                            }
                        }else{
                            resultMap.put("nutritionCondition","重度富营养");
                            for(int i = 0 ; i < gradeList.size() ; i++){
                                if("重度富营养".equals(gradeList.get(i).get("gradeName"))){
                                    resultMap.put("gradeColor",gradeList.get(i).get("gradeColor"));
                                }
                            }
                        }
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
            if (ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
        return resultMap;
    }

    @Override
    public List<Map<String, Object>> getReservoirSZPJNutritionCount(String water_body_id, String begin_year, String end_year, String itemIDs) throws SQLException {
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        for(int j = Integer.valueOf(begin_year) ; j <= Integer.valueOf(end_year) ; j++){
            Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
            Map<String,Object> tempMap = getReservoirSZPJNutrition(water_body_id,j+"",itemIDs);
            if(tempMap.size()==0||tempMap==null){
                resultMap.put("skId","4c5172e3e578452e80c4a7a345773eb4");
                resultMap.put("skName","于桥水库");
                resultMap.put("nutritionCondition","");
                resultMap.put("gradeColor","");
            }else{
                resultMap.putAll(tempMap);
            }
            resultMap.put("year",j);
            resultList.add(resultMap);
        }
        return resultList;
    }

    @Override
    public List<Map<String, String>> getWangegeData() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map map = null;
        //存放结果
        List<Map<String,String>> itemsList = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql ="select OBJ_ID,计算目标_ID,L_CODE,I_CODE,J_CODE,DXDY_DX,DXDY_DY,DXDY_DEPTH,DXDY_ELEV,DXDY_ZROUGH," +
                    "LXLY_X,LXLY_Y,LXLY_CUE,LXLY_CVE,LXLY_CUN,LXLY_CVN,LXLY_WIND,CON_BL_X,CON_BL_Y,CON_TL_X,CON_TL_Y," +
                    "CON_TR_X,CON_TR_Y,CON_BR_X,CON_BR_Y,经度,纬度,SUPER_OBJ_ID,MAP_ID,L_NAME,UTM_X,UTM_Y from yqmx_sys_网格信息表";
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<String,String>();
                map.put("OBJ_ID", rs.getString(1));
                map.put("计算目标_ID", rs.getString(2));
                map.put("L_CODE", rs.getString(3));
                map.put("I_CODE", rs.getString(4));
                map.put("J_CODE", rs.getString(5));
                map.put("DXDY_DX", rs.getString(6));
                map.put("DXDY_DY", rs.getString(7));
                map.put("DXDY_DEPTH", rs.getString(8));
                map.put("DXDY_ELEV", rs.getString(9));
                map.put("DXDY_ZROUGH", rs.getString(10));
                map.put("LXLY_X", rs.getString(11));
                map.put("LXLY_Y", rs.getString(12));
                map.put("LXLY_CUE", rs.getString(13));
                map.put("LXLY_CVE", rs.getString(14));
                map.put("LXLY_CUN", rs.getString(15));
                map.put("LXLY_CVN", rs.getString(16));
                map.put("LXLY_WIND", rs.getString(17));
                map.put("CON_BL_X", rs.getString(18));
                map.put("CON_BL_Y", rs.getString(19));
                map.put("CON_TL_X", rs.getString(20));
                map.put("CON_TL_Y", rs.getString(21));
                map.put("CON_TR_X", rs.getString(22));
                map.put("CON_TR_Y", rs.getString(23));
                map.put("CON_BR_X", rs.getString(24));
                map.put("CON_BR_Y", rs.getString(25));
                map.put("经度", rs.getString(26));
                map.put("纬度", rs.getString(27));
                map.put("SUPER_OBJ_ID", rs.getString(28));
                map.put("MAP_ID", rs.getString(29));
                map.put("L_NAME", rs.getString(30));
                map.put("UTM_X", rs.getString(31));
                map.put("UTM_Y", rs.getString(32));
                itemsList.add(map);
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
        return itemsList;
    }

}
