package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.Md5Util;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.util.StringUtils;

import javax.sql.DataSource;
import java.io.*;
import java.math.BigInteger;
import java.sql.*;
import java.util.*;

/**
 * Created by zsz on 2016/6/22.
 */
@Repository
public class ExecuteExeSZPJDaoImpl implements bitc.sz.wzEnv.dao.ExecuteExeSZPJDao {

    @Autowired
    private DataSource basicDataSource;

    @Override
    public boolean readWQPLT2Database(List<String[]> list,long date, int countNum, String step_id)throws Exception{
        String sql ="insert into SDLM_OUTPUT_计算结果表 (sn_id,step_id,result_date,计算断面号,计算项目_id,计算结果_value) values(?,?,?,?,?,?)";
        String wrwdata = getSzpjData(step_id,"F48D10422AFB430298B879E74920D5A7");
        String[] wrws  = wrwdata.split(";");
        List<String> list1 =new ArrayList<>();
        list1.add("A8E50747D5104EEGA0D6DE52CE82A15A");//距起始断面长度
        for(int i=0;i<wrws.length;i++){
            String wrw = wrws[i].split(",")[0];
            String value = getWrwIdByName(wrw);
            if(!StringUtils.isEmpty(value)){
                list1.add(value);
            }
        }
        String jisuanId[] = new String[list1.size()];
        list1.toArray(jisuanId);
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection=basicDataSource.getConnection();
            connection.setAutoCommit(false);
            ps = connection.prepareStatement(sql);
            String line[] =null;
            for(int i=0;i<countNum;i++){
                line = list.get(i);
                for(int j=0;j<jisuanId.length;j++){
                    ps.setString(1, Md5Util.getUUID());
                    ps.setString(2, step_id);
                    ps.setDate(3, new java.sql.Date(date));
                    ps.setInt(4, Integer.parseInt(line[0]));
                    ps.setString(5, jisuanId[j]);
                    ps.setString(6, line[j+1]);
                    ps.addBatch();
                }
            }
            System.out.println("保存数据...............");
            ps.executeBatch();
            connection.commit();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public List<Map<String, String>> getDMJiceValue(String dmId, String itemId, String start, String end) throws SQLException {
        String sql = "select to_char(a.观测时间,'yyyy-MM-dd') as 时间, round(avg(a.观测值),2)\n" +
                "  from szpj_biz_水质指标观测数据 a\n" +
                "  left join sdlm_sys_断面信息表 c\n" +
                "    on a.观测点_id = c.水资源断面_id\n" +
                " where a.观测item_id = ? and c.obj_id = ? \n" +
                "    and a.观测时间 >= to_date(?,'yyyy-mm-dd') and a.观测时间<= to_date(?,'yyyy-mm-dd')\n" +
                "   group by  to_char(a.观测时间,'yyyy-MM-dd') order by 时间 asc\n";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,itemId);
            ps.setString(2,dmId);
            ps.setString(3, start);
            ps.setString(4, end);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                map.put("time",rs.getString(1));
                map.put("value",rs.getString(2));
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
    public List<Map<String, String>> waterHDData(String planId, String modelId, String juli, String jisuanId, String item_id, String start, String end) throws Exception {
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";

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
                    int year = 0;
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
            String sql = "select a.value,b.color_value from  szpj_sys_指标达标值 a left join szpj_sys_断面水质定性评价 b on a.level_id = b.level_id where a.item_id = ? order by a.value asc";
            String sql1 = "select chardate, c.距离, round(avg(计算值),2) as val,d.obj_id,d.super_obj_id,d.空间数据编号,d.断面名称 \n" +
                    " from (select a.计算断面号,\n" +
                    "                               a.result_date,\n" +
                    "                               a.chardate,\n" +
                    "                               a.计算结果_value as 距离,\n" +
                    "                               b.计算结果_value as 计算值\n" +
                    "                          from (select 计算项目_id,\n" +
                    "                                       计算断面号,\n" +
                    "                                       result_date,\n" +
                    "                                       to_char(result_date, 'yyyy-mm') as chardate,\n" +
                    "                                       计算结果_value\n" +
                    "                                  from sdlm_output_计算结果表\n" +
                    "                                 where 计算项目_id = ? \n" +
                    "                                   and step_id = ? \n" +
                    "                                   and result_date >= to_date(?, 'yyyy-MM-dd HH24:mi:ss')\n" +
                    "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) a,\n" +
                    "                               (select 计算项目_id,\n" +
                    "                                       计算断面号,\n" +
                    "                                       result_date,\n" +
                    "                                       to_char(result_date, 'yyyy-mm') as chardate,\n" +
                    "                                       计算结果_value\n" +
                    "                                  from sdlm_output_计算结果表\n" +
                    "                                 where 计算项目_id = ? \n" +
                    "                                   and step_id = ? \n" +
                    "                                   and result_date >= to_date(? , 'yyyy-MM-dd HH24:mi:ss')\n" +
                    "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) b\n" +
                    "                         where a.计算断面号 = b.计算断面号\n" +
                    "                           and a.result_date = b.result_date ) c " +
                    " left join sdlm_sys_断面信息表 d on c.计算断面号 = d.计算断面号 \n" +
                    "  group by c.计算断面号, c.chardate, c.距离,d.obj_id,d.断面名称,d.super_obj_id,d.空间数据编号 order by chardate asc, 距离 asc  ";
                Map colorMap = null;
                ps = connection.prepareStatement(sql);
                ps.setString(1, item_id);
                rs = ps.executeQuery();
                List<Map<String, String>> colorList = new ArrayList<>();
                while (rs.next()){
                    colorMap = new HashMap<String, String>();
                    colorMap.put(rs.getString(1), rs.getString(2));
                    colorList.add(colorMap);
                }

                ps = connection.prepareStatement(sql1);
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
                    dataMap.put("value", getNmuber(rs.getString(3),true,false));
                    dataMap.put("obj_id", rs.getString(4));
                    dataMap.put("super_obj_id", rs.getString(5));
                    dataMap.put("空间数据id", rs.getString(6));
                    dataMap.put("colo_value", getValueColor(rs.getString(3), colorList));
                    dataMap.put("dmName", rs.getString(7));
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
    public List<Map<String, String>> waterHDDataByDay(String planId, String modelId, String juli, String jisuanId, String item_id, String start, String end) throws Exception {
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";

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
                    int year = 0;
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
            String sql = "select a.value,b.color_value from  szpj_sys_指标达标值 a left join szpj_sys_断面水质定性评价 b on a.level_id = b.level_id where a.item_id = ? order by a.value asc";
            String sql1 = "select chardate, c.距离,计算值 as val,d.obj_id,d.super_obj_id,d.空间数据编号,d.断面名称 \n" +
                    " from (select a.计算断面号,\n" +
                    "                               a.result_date,\n" +
                    "                               a.chardate,\n" +
                    "                               a.计算结果_value as 距离,\n" +
                    "                               b.计算结果_value as 计算值\n" +
                    "                          from (select 计算项目_id,\n" +
                    "                                       计算断面号,\n" +
                    "                                       result_date,\n" +
                    "                                       to_char(result_date, 'yyyy-mm-dd') as chardate,\n" +
                    "                                       计算结果_value\n" +
                    "                                  from sdlm_output_计算结果表\n" +
                    "                                 where 计算项目_id = ? \n" +
                    "                                   and step_id = ? \n" +
                    "                                   and result_date >= to_date(?, 'yyyy-MM-dd HH24:mi:ss')\n" +
                    "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) a,\n" +
                    "                               (select 计算项目_id,\n" +
                    "                                       计算断面号,\n" +
                    "                                       result_date,\n" +
                    "                                       to_char(result_date, 'yyyy-mm-dd') as chardate,\n" +
                    "                                       计算结果_value\n" +
                    "                                  from sdlm_output_计算结果表\n" +
                    "                                 where 计算项目_id = ? \n" +
                    "                                   and step_id = ? \n" +
                    "                                   and result_date >= to_date(? , 'yyyy-MM-dd HH24:mi:ss')\n" +
                    "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) b\n" +
                    "                         where a.计算断面号 = b.计算断面号\n" +
                    "                           and a.result_date = b.result_date ) c " +
                    " left join sdlm_sys_断面信息表 d on c.计算断面号 = d.计算断面号 \n" +
                    "   order by chardate asc, 距离 asc  ";
            Map colorMap = null;
            ps = connection.prepareStatement(sql);
            ps.setString(1, item_id);
            rs = ps.executeQuery();
            List<Map<String, String>> colorList = new ArrayList<>();
            while (rs.next()){
                colorMap = new HashMap<String, String>();
                colorMap.put(rs.getString(1), rs.getString(2));
                colorList.add(colorMap);
            }

            ps = connection.prepareStatement(sql1);
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
                dataMap.put("date", currentMonth);
                dataMap.put("distance", getNmuber(rs.getString(2), true, false));
                dataMap.put("value", getNmuber(rs.getString(3),true,false));
                dataMap.put("obj_id", rs.getString(4));
                dataMap.put("super_obj_id", rs.getString(5));
                dataMap.put("空间数据id", rs.getString(6));
                dataMap.put("colo_value", getValueColor(rs.getString(3), colorList));
                dataMap.put("dmName", rs.getString(7));
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
    public boolean getExportData(String step_id,String filePath) throws SQLException {
        Connection connection = null;
        PreparedStatement ps1 = null;
        ResultSet rs1 = null;
        PreparedStatement ps2 = null;
        ResultSet rs2 = null;
        List<Map<String,Object>> list = new ArrayList<Map<String, Object>>();
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "select * from sdlm_sys_水质断面对应 t order by t.水质归属河段 asc,t.水质计算断面号 asc";
            String sql2 = "select t2.key_abbr, t2.key_name, t1.value\n" +
                    "  from sdlm_biz_方案条件设定表 t1, sdlm_sys_条件项目定义表 t2\n" +
                    " where t1.step_id = ?\n" +
                    "   and t1.水质断面对应_id = ?\n" +
                    "   and t1.key = t2.key_id\n" +
                    "   order by t2.key_name asc";
            ps1 = connection.prepareStatement(sql1);
            ps2 = connection.prepareStatement(sql2);
            rs1 = ps1.executeQuery();
            Set<String> startKeySet = new HashSet<String>();
            Set<String> dischargeKeySet = new HashSet<String>();
            List<String> startKeyList = null;
            List<String> dischargeKeyList = null;
            while (rs1.next()){
                Map<String,Object> rowDataMap = new LinkedHashMap<String,Object>();
                String dmId = rs1.getString(1);
                //河段和断面号列
                rowDataMap.put("Segment",rs1.getString(5));
                rowDataMap.put("Section",rs1.getString(4));

                ps2.setString(1,step_id);
                ps2.setString(2,dmId);
                rs2 = ps2.executeQuery();
                List<Map<String,Object>> tempList = new ArrayList<Map<String, Object>>();
                while (rs2.next()){
                    Map<String,Object> tempMap = new LinkedHashMap<String,Object>();
                    tempMap.put("key",rs2.getString(1));
                    tempMap.put("keyName",rs2.getString(2));
                    tempMap.put("value",rs2.getDouble(3));
                    tempList.add(tempMap);
                }
                //断面距离列
                for(int i = 0 ; i < tempList.size() ; i++){
                    if("断面距离".equals(tempList.get(i).get("keyName"))){
                        rowDataMap.put("distance",tempList.get(i).get("value"));
                    }
                }
                //整理初始浓度和排放浓度列
                List<Map<String,Object>> start = new ArrayList<Map<String, Object>>();
                List<Map<String,Object>> discharge = new ArrayList<Map<String,Object>>();
                for(int i = 0 ; i < tempList.size() ; i++){
                    if(((String)tempList.get(i).get("keyName")).contains("初始浓度")){
                        start.add(tempList.get(i));
                    }
                    if(((String)tempList.get(i).get("keyName")).contains("排放浓度")){
                        discharge.add(tempList.get(i));
                    }
                }
                //初始浓度列
                for(int i = 0 ; i < start.size() ; i++){
                    startKeySet.add((String)start.get(i).get("key"));
                    rowDataMap.put((String)start.get(i).get("key"),start.get(i).get("value"));
                }
                //污染物流量列
                for(int i = 0 ; i < tempList.size() ; i++){
                    if("污染物流量".equals(tempList.get(i).get("keyName"))){
                        rowDataMap.put("flow", tempList.get(i).get("value"));
                    }
                }
                //排放浓度列
                for(int i = 0 ; i < discharge.size() ; i++){
                    dischargeKeySet.add((String)discharge.get(i).get("key"));
                    rowDataMap.put((String)discharge.get(i).get("key"),discharge.get(i).get("value"));
                }
                list.add(rowDataMap);
            }
            startKeyList = new ArrayList<String>(startKeySet);
            dischargeKeyList = new ArrayList<String>(dischargeKeySet);
            //对于startKeyList，dischargeKeyList排序
            asc(startKeyList,6);
            asc(dischargeKeyList, 6);
            //---------------------------------------------------------------------------
            //开始进行输出文件

            File file1 = new File(filePath);
            if(!file1.exists()){
                file1.mkdirs();
            }


            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath+"/DXQC.TXT")));

            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append("Segment" + "  ");
            stringBuilder.append("Section" + "  ");
            stringBuilder.append("distance" + "  ");
            for(int k = 0 ; k < startKeyList.size() ; k++){
                stringBuilder.append(startKeyList.get(k) + "  ");
            }
            stringBuilder.append("flow" + "  ");
            for(int k = 0 ; k < dischargeKeyList.size() ; k++){
                stringBuilder.append(dischargeKeyList.get(k)+"  ");
            }
            bufferedWriter.write(stringBuilder.toString());
            bufferedWriter.newLine();

            for(int j = 0 ; j < list.size() ; j++){
                Map<String,Object> map = list.get(j);
                StringBuilder str = new StringBuilder();
                str.append(map.get("Segment")+"\t");
                str.append(map.get("Section")+"\t");
                str.append(map.get("distance"));
                str.append("\t");
                for(int k = 0 ; k < startKeyList.size() ; k++){
                    str.append(map.get(startKeyList.get(k)));
                    str.append("\t");
                }
                str.append(map.get("flow"));
                str.append("\t");
                for(int k = 0 ; k < dischargeKeyList.size() ; k++){
                    str.append(map.get(dischargeKeyList.get(k)));
                    str.append("\t");
                }
                bufferedWriter.write(str.toString());
                bufferedWriter.newLine();
            }
            bufferedWriter.flush();
            bufferedWriter.close();
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            if(rs1 != null){
                rs1.close();
            }
            if(rs2 != null){
                rs2.close();
            }
            if(ps1 != null){
                ps1.close();
            }
            if(ps2 != null){
                ps2.close();
            }
            connection.close();
        }
        return false;
    }

    @Override
    public boolean exportCBOUData(String step_id, String filePath) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        List<Map<String,String>> list = new ArrayList<Map<String, String>>();
        try {
            connection = basicDataSource.getConnection();
            String sql = "   select value \n" +
                    "     from sdlm_biz_方案条件设定表\n" +
                    "    where step_id = ?\n" +
                    "      and key = '04C2A020B5C341189BAF1364810A7780'\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,step_id);
            rs = ps.executeQuery();
            if(rs.next()){
                value = rs.getString(1);
            }
            File file1 = new File(filePath);
            if(!file1.exists()){
                file1.mkdirs();
            }
            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath+"/CBOU.TXT")));
            bufferedWriter.write("-----------------");
            bufferedWriter.newLine();
            ArrayList<String[]> arrayList = new ArrayList();
            if(value != null){
                String[] valueArr1 = value.split(";");
                for(int i = 0 ; i < valueArr1.length ; i++){
                    String[] valueArr2 = valueArr1[i].split(",");
                    arrayList.add(valueArr2);
                }
                String[][] valueArr = (String [][])arrayList.toArray(new String[0][0]);
                //计算valueArr这个二维数组的行数和列数
                int rowCount = valueArr.length;
                int colCount = 0;
                for(int  i = 0 ; i < rowCount; i++){
                    if(valueArr[i].length>colCount){
                        colCount = valueArr[i].length;
                    }
                }

                Set<String> key = new HashSet<String>();
                for(int i = 1 ; i < colCount ;i++){
                    Map<String,String> rowDataMap = new LinkedHashMap<String,String>();
                    for(int j = 0 ; j < rowCount ; j++){
                        rowDataMap.put(valueArr[j][0], valueArr[j][i]);
                        key.add(valueArr[j][0]);
                    }
                    list.add(rowDataMap);
                }

                int count = 1;
                for(int k = 0 ; k < list.size() ; k++){
                    StringBuilder str = new StringBuilder();
                    Iterator keys = key.iterator();
                    if(count==1){
                        str.append("1"+"\t"+"1"+"\t");
                    }
                    if(count==2){
                        str.append("3"+"\t"+"70"+"\t");
                    }
                    Map<String,String> map = list.get(k);
                    while (keys.hasNext()){
                        String currentKey = (String) keys.next();
                        str.append(map.get(currentKey));
                        str.append("\t");
                        str.append(map.get(currentKey));
                        str.append("\t");
                    }
                    bufferedWriter.write(str.toString());
                    bufferedWriter.newLine();
                    count++;
                }

                bufferedWriter.flush();
                bufferedWriter.close();
                return true;
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
            connection.close();
        }
        return false;
    }

    @Override
    public boolean exportCTOTALData(String step_id, String filePath) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            File file1 = new File(filePath);
            if(!file1.exists()){
                file1.mkdirs();
            }
            BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath+"/CTOTAL.DAT")));
            bufferedWriter.write("!--------TAI ZI HE Project Program Test--------");
            bufferedWriter.newLine();
            bufferedWriter.write("!------------------------------------------------");
            bufferedWriter.newLine();
            String[] keyArr = {"sz_nr","sz_nj","sz_njb","sz_nl","sz_dt",
                    "sz_month","sz_mt","sz_nc","sz_njpm","sz_nspm","sz_mpr" ,"sz_GDO","sz_Bdo"};
            String sql = "select t2.key_abbr, t1.value\n" +
                    "  from sdlm_biz_方案条件设定表 t1, sdlm_sys_条件项目定义表 t2\n" +
                    " where t1.step_id = ?\n" +
                    "   and t2.key_abbr = ?\n" +
                    "   and t1.key = t2.key_id\n";
            ps = connection.prepareStatement(sql);
            for (int i = 0 ; i < keyArr.length ; i++){
                ps.setString(1, step_id);
                ps.setString(2, keyArr[i]);
                rs = ps.executeQuery();
                if(rs.next()){
                    if(rs.getString(1).equals("sz_month")){
                        if(!StringUtils.isEmpty(rs.getString(2))){
                            String[] monthArr = rs.getString(2).split(",");
                            bufferedWriter.write("\t");
                            bufferedWriter.write(monthArr.length+"");
                            bufferedWriter.newLine();
                            bufferedWriter.write("\t");
                            for(int j = 0 ; j < monthArr.length ; j++){
                                bufferedWriter.write(monthArr[j]);
                                bufferedWriter.write("\t");
                            }
                            bufferedWriter.newLine();
                        }
                    }else{
                        bufferedWriter.write("\t");
                        bufferedWriter.write(rs.getString(2));
                        bufferedWriter.newLine();
                    }
                }
            }

            bufferedWriter.write("!-------------------------");
            bufferedWriter.newLine();
            for(int i = 0 ; i < 6; i++){
                bufferedWriter.write("\t");
                bufferedWriter.write("0");
                bufferedWriter.newLine();
            }
            bufferedWriter.write("!-------------------------");
            bufferedWriter.newLine();
            String[] keyArr2 = {"sz_wry","sz_nspr","sz_wuranwuIndex"};
            String[] outPutArr = {};
            for(int i = 0 ; i < keyArr2.length ; i++){
                ps.setString(1, step_id);
                ps.setString(2, keyArr2[i]);
                rs = ps.executeQuery();
                if(rs.next()){
                    if(rs.getString(1).equals("sz_wry")){
                        if(!StringUtils.isEmpty(rs.getString(2))){
                            String[] arr = rs.getString(2).split(";");
                            bufferedWriter.write(arr.length+"");
                            bufferedWriter.newLine();
                            for(int j = 0 ; j < arr.length ; j++){
                                bufferedWriter.write(arr[j]+"\t");
                            }
                            bufferedWriter.newLine();
                            bufferedWriter.write("!-------------------------");
                            bufferedWriter.newLine();
                        }
                    }else if(rs.getString(1).equals("sz_nspr")){
                        if(!StringUtils.isEmpty(rs.getString(2))){
                            outPutArr = rs.getString(2).split(";");
                            bufferedWriter.write("\t");
                            bufferedWriter.write(outPutArr.length+"");
                            bufferedWriter.newLine();
                            bufferedWriter.write("!-------------------------");
                            bufferedWriter.newLine();
                        }
                    }else{
                        if(!StringUtils.isEmpty(rs.getString(2))){
                            String[] arr = rs.getString(2).split(",");
                            for(int j = 0 ; j < arr.length ; j++){
                                bufferedWriter.write("\t");
                                bufferedWriter.write(arr[j]);
                            }
                            bufferedWriter.newLine();
                            bufferedWriter.write("!-------------------------");
                            bufferedWriter.newLine();
                        }
                    }
                }
            }
            ArrayList<String[]> arrayList = new ArrayList();
            for(int k = 0 ; k < outPutArr.length ; k++){
                String[] valueArr2 = outPutArr[k].split(",");
                arrayList.add(valueArr2);
            }
            String[][] arr = arrayList.toArray(new String[0][0]);
            //计算valueArr这个二维数组的行数和列数
            int rowCount = arr.length;
            int colCount = 0;
            for(int  i = 0 ; i < rowCount; i++){
                if(arr[i].length>colCount){
                    colCount = arr[i].length;
                }
            }
            List<Map<String,String>> list = new ArrayList<Map<String, String>>();
            Set<String> key = new HashSet<String>();
            for(int i = 0 ; i < colCount ;i++){
                Map<String,String> rowDataMap = new LinkedHashMap<String,String>();
                for(int j = 0 ; j < rowCount ; j++){
                    rowDataMap.put("key"+j, arr[j][i]);
                    key.add("key"+j);
                }
                list.add(rowDataMap);
            }
            for(int k = 0 ; k < list.size() ; k++){
                StringBuilder str = new StringBuilder();
                Iterator keys = key.iterator();
                Map<String,String> map = list.get(k);
                while (keys.hasNext()){
                    String currentKey = (String) keys.next();
                    str.append(map.get(currentKey));
                    str.append("\t");
                }
                bufferedWriter.write(str.toString());
                bufferedWriter.newLine();
            }
            //--------------------------------------------------------
            bufferedWriter.write("!------------------------- ");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("28*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("30*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("30*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("30*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();
            bufferedWriter.write("30*20");
            bufferedWriter.newLine();
            bufferedWriter.write("31*20");
            bufferedWriter.newLine();


            bufferedWriter.flush();
            bufferedWriter.close();
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(rs!=null){
                rs.close();
            }
            if(ps!=null){
                ps.close();
            }
            connection.close();
        }
        return false;
    }

    @Override
    public String getStepByPlan(String plan_id) throws SQLException {
        String sql = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = '04b71ddcb30a454ga9ede57198834243'";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String step_id = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,plan_id);
            rs = ps.executeQuery();
            if (rs.next()){
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
    @Override
    public String getWrwIdByName(String key) throws SQLException {
        String sql = "select 计算项目_id from sdlm_sys_计算结果项目表 where 计算项目名称 = ? ";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,key);
            rs = ps.executeQuery();
            if (rs.next()){
                value = rs.getString(1);
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
        return value;
    }

    @Override
    public int getYearByPlan(String plan_id) throws SQLException {
        String sql = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ? ";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        int value = 0;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,plan_id);
            rs = ps.executeQuery();
            if (rs.next()){
                value = rs.getInt(1);
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
        return value;
    }

    @Override
    public boolean exportSsCTOTALData(String step_id, String filePath) throws SQLException {
            Connection connection = null;
            PreparedStatement ps = null;
            ResultSet rs = null;
            try {
                connection = basicDataSource.getConnection();
                File file1 = new File(filePath);
                if(!file1.exists()){
                    file1.mkdirs();
                }
                BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath+"/CTOTAL.DAT")));
                bufferedWriter.write("!--------TAI ZI HE Project Program Test--------");
                bufferedWriter.newLine();
                bufferedWriter.write("!------------------------------------------------");
                bufferedWriter.newLine();
                String[] keyArr = {"sz_nr","sz_nj","sz_njb","sz_nl","sz_shijianjiange","sz_dt",
                        "sz_month","sz_mt","sz_nc","sz_njpm","sz_nspm","sz_mpr" ,"sz_GDO","sz_Bdo"};
                String sql = "select t2.key_abbr, t1.value\n" +
                        "  from sdlm_biz_方案条件设定表 t1, sdlm_sys_条件项目定义表 t2\n" +
                        " where t1.step_id = ?\n" +
                        "   and t2.key_abbr = ?\n" +
                        "   and t1.key = t2.key_id\n";
                ps = connection.prepareStatement(sql);
                for (int i = 0 ; i < keyArr.length ; i++){
                    ps.setString(1, step_id);
                    ps.setString(2, keyArr[i]);
                    rs = ps.executeQuery();
                    if(rs.next()){
                        if(rs.getString(1).equals("sz_month")){
                            if(!StringUtils.isEmpty(rs.getString(2))){
                                String[] monthArr = rs.getString(2).split(",");
                                bufferedWriter.write("\t");
                                bufferedWriter.write(monthArr.length+"");
                                bufferedWriter.newLine();
                                bufferedWriter.write("\t");
                                for(int j = 0 ; j < monthArr.length ; j++){
                                    bufferedWriter.write(monthArr[j]);
                                    bufferedWriter.write("\t");
                                }
                                bufferedWriter.newLine();
                            }
                        }else{
                            bufferedWriter.write("\t");
                            bufferedWriter.write(rs.getString(2));
                            bufferedWriter.newLine();
                        }
                    }
                }

                bufferedWriter.write("!-------------------------");
                bufferedWriter.newLine();
                for(int i = 0 ; i < 6; i++){
                    bufferedWriter.write("\t");
                    bufferedWriter.write("0");
                    bufferedWriter.newLine();
                }
                bufferedWriter.write("!-------------------------");
                bufferedWriter.newLine();
                String[] keyArr2 = {"sz_wry","sz_nspr","sz_wuranwuIndex"};
                String[] outPutArr = {};
                bufferedWriter.write("1");
                bufferedWriter.newLine();
                for(int i = 0 ; i < keyArr2.length ; i++){
                    ps.setString(1, step_id);
                    ps.setString(2, keyArr2[i]);
                    rs = ps.executeQuery();
                    if(rs.next()){
                        if(rs.getString(1).equals("sz_wry")){
                            if(!StringUtils.isEmpty(rs.getString(2))){
                                String[] arr = rs.getString(2).split(";");
                                bufferedWriter.write(arr.length+"");
                                bufferedWriter.newLine();
                                for(int j = 0 ; j < arr.length ; j++){
                                    bufferedWriter.write(arr[j]+"\t");
                                }
                                bufferedWriter.newLine();
                                bufferedWriter.write("!-------------------------");
                                bufferedWriter.newLine();
                            }
                        }else if(rs.getString(1).equals("sz_nspr")){
                            if(!StringUtils.isEmpty(rs.getString(2))){
                                outPutArr = rs.getString(2).split(";");
                                bufferedWriter.write("\t");
                                bufferedWriter.write(outPutArr.length+"");
                                bufferedWriter.newLine();
                                bufferedWriter.write("!-------------------------");
                                bufferedWriter.newLine();
                            }
                        }else{
                            if(!StringUtils.isEmpty(rs.getString(2))){
                                String[] arr = rs.getString(2).split(",");
                                for(int j = 0 ; j < arr.length ; j++){
                                    bufferedWriter.write("\t");
                                    bufferedWriter.write(arr[j]);
                                }
                                bufferedWriter.newLine();
                                bufferedWriter.write("!-------------------------");
                                bufferedWriter.newLine();
                            }
                        }
                    }
                }
                bufferedWriter.write("\t1\t1");
                bufferedWriter.newLine();

                bufferedWriter.write("\t3\t70");
                bufferedWriter.newLine();

                bufferedWriter.write("!-------------------------");
                bufferedWriter.newLine();

                ArrayList<String[]> arrayList = new ArrayList();
                for(int k = 0 ; k < outPutArr.length ; k++){
                    String[] valueArr2 = outPutArr[k].split(",");
                    arrayList.add(valueArr2);
                }
                String[][] arr = arrayList.toArray(new String[0][0]);
                //计算valueArr这个二维数组的行数和列数
                int rowCount = arr.length;
                int colCount = 0;
                for(int  i = 0 ; i < rowCount; i++){
                    if(arr[i].length>colCount){
                        colCount = arr[i].length;
                    }
                }
                List<Map<String,String>> list = new ArrayList<Map<String, String>>();
                Set<String> key = new HashSet<String>();
                for(int i = 0 ; i < colCount ;i++){
                    Map<String,String> rowDataMap = new LinkedHashMap<String,String>();
                    for(int j = 0 ; j < rowCount ; j++){
                        rowDataMap.put("key"+j, arr[j][i]);
                        key.add("key"+j);
                    }
                    list.add(rowDataMap);
                }
                for(int k = 0 ; k < list.size() ; k++){
                    StringBuilder str = new StringBuilder();
                    Iterator keys = key.iterator();
                    Map<String,String> map = list.get(k);
                    while (keys.hasNext()){
                        String currentKey = (String) keys.next();
                        str.append(map.get(currentKey));
                        str.append("\t");
                    }
                    bufferedWriter.write(str.toString());
                    bufferedWriter.newLine();
                }
                //--------------------------------------------------------
                bufferedWriter.write("!------------------------- ");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("28*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("30*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("30*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("30*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();
                bufferedWriter.write("30*20");
                bufferedWriter.newLine();
                bufferedWriter.write("31*20");
                bufferedWriter.newLine();


                bufferedWriter.flush();
                bufferedWriter.close();
                return true;
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if(rs!=null){
                    rs.close();
                }
                if(ps!=null){
                    ps.close();
                }
                connection.close();
            }
            return false;
    }

    @Override
    public List<Map<String, String>> getWuRanWuList(String planId) throws SQLException {
        String sql = "select value from SDLM_BIZ_方案条件设定表 a left join sdlm_biz_方案步骤定义 b on a.step_id = b.step_id where a.key=? and b.plan_id= ? and b.model_code='04b71ddcb30a454ga9ede57198834243'";
        String sql1="select 计算项目_id from sdlm_sys_计算结果项目表 where 计算项目名称 = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        PreparedStatement ps1 = null;
        ResultSet rs = null;
        ResultSet rs1 = null;
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String,String> map = null;
        String indexs="";
        String names="";
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps1 = connection.prepareStatement(sql1);
            ps.setString(1,"AD018F05C83240B59D43A1E304AA6E08"); //下标
            ps.setString(2,planId);
            rs = ps.executeQuery();
            if (rs.next()){
                indexs = rs.getString(1);
                ps.setString(1,"99139BDD160A4F75B31241C5FA6CC738"); //污染物集合
                ps.setString(2,planId);
                rs = ps.executeQuery();
                if(rs.next()){
                    names = rs.getString(1);
                    String [] index = indexs.split(",");
                    String [] name = names.split(",");
                    for(int i  = 0;i<index.length && i< name.length ;i++){
                        map = new HashMap<>();
                        if(!"0".equals(index[i])){
                            ps1.setString(1,"wrw"+(i+1));
                            rs1 = ps1.executeQuery();
                            if(rs1.next()){
                                map.put("id",rs1.getString(1));
                            }
                            map.put("name",name[i]);
                            list.add(map);
                        }
                    }
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
    public String getJianCeItemId(String name) throws SQLException {
        String sql = "select item_id from szpj_sys_指标项 where sys_name = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,name);
            rs = ps.executeQuery();
            if (rs.next()){
                value = rs.getString(1);
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
        return value == null?"5F769D7EA7F54E509209E0B869301080":value;
    }

    @Override
    public void updatePlanState(String plan_id,int i) throws SQLException {
        String sql = "update sdlm_biz_方案定义表 set 临时状态标识 = ? where plan_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setInt(1,i);
            ps.setString(2,plan_id);
            ps.execute();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }
    @Override
    public String getSzpjData(String step_id,String key) throws SQLException {
        String sql = "select value from sdlm_biz_方案条件设定表 where step_id = ? and key = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, step_id);
            ps.setString(2,key);
            rs = ps.executeQuery();
            if (rs.next()){
                value = rs.getString(1);
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
        return value;
    }
    public List<String> asc (List<String> list, final int index){
        Collections.sort(list, new Comparator<String>() {
            public int compare(String o1, String o2) {
                int map1value = o1.charAt(index);
                int map2value = o2.charAt(index);
                return map1value - map2value;
            }
        });
        return list;
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
    public List<Map<String, String>> waterHDSsSource(String step_id, String juliId, String jisuanId, String item_id, String start, String end) throws SQLException {
       String sql = "select a.value,b.color_value from  szpj_sys_指标达标值 a left join szpj_sys_断面水质定性评价 b on a.level_id = b.level_id where a.item_id = ? order by a.value asc";
        String sql1 = "select chardate,c.距离, 计算值 as val,d.obj_id,d.super_obj_id,d.空间数据编号,d.断面名称 \n" +
                " from (select a.计算断面号,\n" +
                "                               a.chardate,\n" +
                "                               a.计算结果_value as 距离,\n" +
                "                               b.计算结果_value as 计算值\n" +
                "                          from (select 计算项目_id,\n" +
                "                                       计算断面号,\n" +
                "                                       to_char(result_date, 'yyyy-MM-dd') as chardate,\n" +
                "                                       计算结果_value\n" +
                "                                  from sdlm_output_计算结果表\n" +
                "                                 where 计算项目_id = ? \n" +
                "                                   and step_id = ? \n" +
                "                                   and result_date >= to_date(?, 'yyyy-MM-dd HH24:mi:ss')\n" +
                "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) a,\n" +
                "                               (select 计算项目_id,\n" +
                "                                       计算断面号,\n" +
                "                                       to_char(result_date, 'yyyy-MM-dd') as chardate,\n" +
                "                                       计算结果_value\n" +
                "                                  from sdlm_output_计算结果表\n" +
                "                                 where 计算项目_id = ? \n" +
                "                                   and step_id = ? \n" +
                "                                   and result_date >= to_date(? , 'yyyy-MM-dd HH24:mi:ss')\n" +
                "                                   and result_date <= to_date(?, 'yyyy-MM-dd HH24:mi:ss')) b\n" +
                "                         where a.计算断面号 = b.计算断面号\n" +
                "                           and a.chardate = b.chardate) c " +
                " left join sdlm_sys_断面信息表 d on c.计算断面号 = d.计算断面号 \n" +
                "  order by c.chardate asc, 距离 asc  ";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String, String>> dataList = new ArrayList<>();
        try {
            Map dataMap = null;
            Map colorMap = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, item_id);
            rs = ps.executeQuery();
            List<Map<String, String>> colorList = new ArrayList<>();
            while (rs.next()){
                colorMap = new HashMap<String, String>();
                colorMap.put(rs.getString(1), rs.getString(2));
                colorList.add(colorMap);
            }

            ps = connection.prepareStatement(sql1);
            ps.setString(1, juliId);
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
                dataMap.put("value", getNmuber(rs.getString(3),true,false));
                dataMap.put("obj_id", rs.getString(4));
                dataMap.put("super_obj_id", rs.getString(5));
                dataMap.put("空间数据id", rs.getString(6));
                dataMap.put("colo_value", getValueColor(rs.getString(3), colorList));
                dataMap.put("dmName", rs.getString(7));
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
    public int getPlanState(String plan_id) throws SQLException {
        String sql = "select 临时状态标识 from sdlm_biz_方案定义表  where plan_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,plan_id);
            rs = ps.executeQuery();
            if(rs.next()){
                return rs.getInt(1);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null){
                rs.close();
            }
            if(ps != null) ps.close();
            if(connection != null)  connection.close();
        }
        return 0;
    }

    private String getValueColor(String value,List<Map<String, String>> colorList){
        if(!StringUtils.isEmpty(value)){
            float valueFloat = Float.valueOf(value);
            Map<String, String> map = null;
            for(int i=0;i<colorList.size();i++){
                map = colorList.get(i);
                Iterator<String> it  = map.keySet().iterator();
                if(it.hasNext()){
                    String key = it.next();
                    float comValue = Float.valueOf(key);
                    if(valueFloat <= comValue){
                        return map.get(key);
                    }
                }
            }
            return "0xFF3F00";
        }
        return null;
    }
}
