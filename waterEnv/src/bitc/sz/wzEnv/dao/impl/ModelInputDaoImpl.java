package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.dao.ModelInputDao;
import bitc.sz.wzEnv.security.service.IAuthService;
import bitc.sz.wzEnv.util.CacheUtil;
import bitc.sz.wzEnv.util.MapSortUtil;
import bitc.sz.wzEnv.util.Md5Util;
import org.apache.commons.collections.map.LinkedMap;
import org.apache.commons.lang.StringUtils;
import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import javax.sql.DataSource;
import java.io.*;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zsz on 2016/7/4.
 */
@Repository
public class ModelInputDaoImpl implements ModelInputDao  {
    @Autowired
    private DataSource basicDataSource;
    @Override
    public List<Map<String,String>> getZhiliuListData(String water_body_id) throws Exception {
        String sql ="select a.file_id,a.file_name,b.obj_id,b.断面名称,b.计算断面号,a.河流名称,b.计算断面号||','||a.file_name from sdlm_sys_参数文件 a left join sdlm_sys_断面信息表 b on a.汇入干流断面_id = b.obj_id where a.是否支流文件 = 1 and  b.计算目标_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                String fileName = rs.getString(2);
                map.put("zhiliuId",rs.getString(1));
                map.put("zhiliuName",fileName.substring(0, fileName.lastIndexOf(".")));
                map.put("dmId",rs.getString(3));
                map.put("dmName",rs.getString(4));
                map.put("dmHao",rs.getString(5));
                map.put("heliuName",rs.getString(6));
                map.put("dmAndFileName",rs.getString(7));
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
    public List<Map<String,String>> getBianjieListData(String water_body_id,String plan_id) throws Exception {
        String sql ="select a.file_id,a.file_name,b.obj_id,b.断面名称,b.计算断面号,a.河流名称 from sdlm_sys_参数文件 a left join sdlm_sys_断面信息表 b on a.汇入干流断面_id = b.obj_id where a.是否支流文件 = 1 and  b.计算目标_id = ?";
        String sql1 = "select a.value from sdlm_biz_方案条件设定表  a left join sdlm_biz_方案步骤定义 b on a.step_id = b.step_id where b.plan_id = ? and a.key = '31BB2E0C29864A908CACC114AC2ACBDB'";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList();
        List<Map<String,String>> list1 = new ArrayList();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                String fileName = rs.getString(2);
                map.put("fileName",fileName);
                map.put("dmName",rs.getString(6));
                list.add(map);
            }
            ps = connection.prepareStatement(sql1);
            ps.setString(1,plan_id);
            rs = ps.executeQuery();
            if(rs.next()){
                String value = rs.getString(1);
                if(!StringUtils.isBlank(value)){
                    for(int i=0;i<list.size();i++){
                        String name = list.get(i).get("fileName");
                        if(value.indexOf(name.substring(0,name.indexOf("."))) > -1){
                            list1.add(list.get(i));
                        }
                    }
                }
            }
            map = new HashMap<>();
            map.put("fileName","_t_TaiZiHQ.dat");
            map.put("dmName", "唐马寨");
            list1.add(map);

            map = new HashMap<>();
            map.put("fileName","_g_TaiZiHQ.dat");
            map.put("dmName", "观音阁");
            list1.add(map);
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
    public List<Map<String,String>> getHedaoListData() throws Exception {
        String sql ="select distinct concat('河道',水质归属河段),水质归属河段 from sdlm_sys_水质断面对应 order by 水质归属河段 asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                map.put("name",rs.getString(1));
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
    public List<Map<String,String>> getDuanMianListData(int hedao) throws Exception {
        String sql ="select concat('断面号',水质计算断面号),水质计算断面号 from sdlm_sys_水质断面对应 where 水质归属河段 = ? order by 水质计算断面号 asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setInt(1,hedao);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                map.put("name",rs.getString(1));
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
    public List<Map<String,String>> getHeDuanListData(String water_body_id) throws Exception {
        String sql ="select distinct(河段名称) from sdlm_sys_断面信息表 where  计算目标_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList();
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, water_body_id);
            rs = ps.executeQuery();
            Map<String,String> map = null;
            while (rs.next()){
                map = new HashMap<>();
                map.put("heduanName",rs.getString(1));
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
    public List<Map<String,Object>> getZhaMenListDate(String water_body_id) throws Exception{
        String sql ="select b.闸门_id,b.闸门_name,a.上游水位,a.下游流量,b.闸门断面号 from sdlm_sys_闸门参数表 a left join sdlm_sys_闸门定义表 b on a.闸门_id = b.闸门_id order by 闸门_id asc,上游水位 asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> list = new ArrayList();
        Map<String,Object> map = null;
        List<String> H = null;
        List<String> Q = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            String name = null;
            while (rs.next()){
                if(name == null){
                    name  = rs.getString(2);
                    map = new HashMap<>();
                    H = new ArrayList();
                    Q = new ArrayList();
                    map.put("id",rs.getString(1));
                    map.put("name",rs.getString(2));
                    map.put("dmId",rs.getInt(5));
                    H.add(rs.getString(3));
                    Q.add(rs.getString(4));
                }else if(name.equals(rs.getString(2))){
                    H.add(rs.getString(3));
                    Q.add(rs.getString(4));
                }else {
                    map.put("shuiwei",H);
                    map.put("liuliang",Q);
                    list.add(map);
                    name = rs.getString(2);
                    map = new HashMap<>();
                    H = new ArrayList();
                    Q = new ArrayList();
                    map.put("id", rs.getString(1));
                    map.put("name",rs.getString(2));
                    map.put("dmId",rs.getInt(5));
                    H.add(rs.getString(3));
                    Q.add(rs.getString(4));
                }
            }
            map.put("shuiwei",H);
            map.put("liuliang",Q);
            list.add(map);
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
    public List<Map<String,String>> getZhaMenNameAndDmId() throws SQLException {
        String sql ="select 闸门_name,闸门断面号 from sdlm_sys_闸门定义表";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList<>();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                map.put("name",rs.getString(1));
                map.put("dmId",rs.getString(2));
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
    public List<Map<String,Object>> getShuiKuListDate(String water_body_id) throws Exception {
        String sql ="select b.水库名称,c.计算断面号 from sdlm_sys_水库对应断面 a left join V_水库基本信息 b on a.水库_id = b.水体_id left join sdlm_sys_断面信息表 c on a.断面_id = c.obj_id where c.计算目标_id =?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> list = new ArrayList<>();
        Map<String,Object> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, water_body_id);
            rs = ps.executeQuery();
            while (rs.next()){
                map = new HashMap<>();
                map.put("shuiku",rs.getString(1));
                map.put("dmId",rs.getInt(2));
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
    public Map<String, String> getInputItemData(String planId, String[] params) throws SQLException {
        String sql = "select value from sdlm_biz_方案条件设定表 a left join sdlm_sys_条件项目定义表 b on a.key = b.key_id where a.step_id = ? and b.KEY_ABBR = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        Map<String,String> map = new HashMap<>();
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            for(int i=0;i<params.length;i++){
                value = "";
                ps.setString(1, planId);
                ps.setString(2,params[i]);
                rs = ps.executeQuery();
                if (rs.next()){
                    value = rs.getString(1);
                }
                map.put(params[i],value);
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
    public List<Map<String, String>> getXulieMapping(String planId) throws SQLException {
        String sql = "select i_code,j_code,ll_xulie,wd_xulie,wrw_xulie from YQMX_BIZ_主控序列对应表 where plan_id = ? order by ll_xulie asc";
        String sql1="select max(序列_id) from yqmx_biz_运算参数表 where 方案_no =? and 参数项目_id =?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,String>> list = new ArrayList<>();
        Map<String,String> map = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1,planId);
            ps.setString(2,"343446D90C3144ABA493952C9975B216");
            rs= ps.executeQuery();
            int wenduXieliecount =0;
            if(rs.next()){
                wenduXieliecount = rs.getInt(1);
            }
            ps.setString(1,planId);
            ps.setString(2,"EA2D29994A254C1B85C5B0C8F53A8648");
            rs= ps.executeQuery();
            int wuranwuXieliecount =0;
            if(rs.next()){
                wuranwuXieliecount = rs.getInt(1);
            }
            map = new HashMap<>();
            map.put("wendu_xulie", String.valueOf(wenduXieliecount));
            map.put("wuranwu_xulie", String.valueOf(wenduXieliecount));
            list.add(map);
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs= ps.executeQuery();
            while(rs.next()){
                map = new HashMap<>();
                map.put("i_code",rs.getString(1));
                map.put("j_code",rs.getString(2));
                map.put("IQSERQ",rs.getString(3));
                map.put("ICSER2",rs.getString(4));
                map.put("ICSER3",rs.getString(5));
                list.add(map);
            }
            return list;
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
    public  List<List<NameValuePair>> getFileItemData(String planId, String itemId) throws SQLException {
        String sql = "select to_char(day,'fm99999999990.000'),to_char(参数_value,'fm99999999990.0000'),序列_id from yqmx_biz_运算参数表 where 方案_no = ? and 参数项目_id =? order by 序列_id asc,day asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,String> map = null;
        List<List<NameValuePair>> list = new ArrayList<>();
        List<NameValuePair> list1 = null;
        try {
            connection=basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.setString(2,itemId);
            rs= ps.executeQuery();
            NameValuePair kv = null;
            String xulie = null;
            while(rs.next()){
                if(xulie == null){
                    xulie = rs.getString(3);
                    list1 = new ArrayList<>();
                    kv = new BasicNameValuePair(rs.getString(1),rs.getString(2));
                    list1.add(kv);
                }else if(xulie.equals(rs.getString(3))){
                    kv = new BasicNameValuePair(rs.getString(1),rs.getString(2));
                    list1.add(kv);
                }else{
                    list.add(list1);
                    xulie = rs.getString(3);
                    list1 = new ArrayList<>();
                    kv = new BasicNameValuePair(rs.getString(1),rs.getString(2));
                    list1.add(kv);
                }
            }
            list.add(list1);
            return list;
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
    public void importVELOutputResult(File file, String planId) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps = null;
        BufferedReader reader1 = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "insert into yqmx_output_计算结果 (SN_ID,VEL_N,VEL_TIME,VEL_DT,VEL_L_CODE,VEL_X_VALUE,VEL_Y_VALUE,VEL_Z_VELUE,STEP_ID) values(?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line ;
            int count = 0;
            List<Integer> countList = new ArrayList<Integer>();
            List<String[]> firstData = new ArrayList<String[]>();

            while ((line = reader.readLine()) != null){
                count++;
                String newLine = line.replaceAll("\\s{1,}", " ");
                if(newLine.contains("N")){
                    String[] arr = newLine.split(" ");
                    String[] tempArr = {arr[2],arr[4],arr[6]};
                    firstData.add(tempArr);
                    countList.add(count);
                }
            }
             reader1 = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line1 ;
            int rowIndex = 0;
            while ((line1 = reader1.readLine()) != null){
                rowIndex++;
                for(int i = 0 ; i < countList.size()-1 ; i++){
                    if(rowIndex>countList.get(i) && rowIndex<countList.get(i+1)){
                        String uuid = Md5Util.getUUID();
                        String newLine = line1.replaceAll("\\s{1,}", " ");
                        String[] arr = newLine.split(" ");
                        String[] firstArr = firstData.get(i);
                        ps.setString(1,uuid);
                        ps.setString(2,firstArr[0]);
                        ps.setString(3,firstArr[1]);
                        ps.setString(4,firstArr[2]);
                        ps.setString(5,arr[1]);
                        ps.setString(6,arr[2]);
                        ps.setString(7,arr[3]);
                        ps.setString(8,arr[4]);
                        ps.setString(9,planId);
                        ps.execute();
                    }
                }
                if(rowIndex>countList.get(countList.size()-1)){
                    String uuid = Md5Util.getUUID();
                    String newLine = line1.replaceAll("\\s{1,}", " ");
                    String[] arr = newLine.split(" ");
                    String[] firstArr = firstData.get(countList.size()-1);
                    ps.setString(1,uuid);
                    ps.setString(2,firstArr[0]);
                    ps.setString(3,firstArr[1]);
                    ps.setString(4,firstArr[2]);
                    ps.setString(5,arr[1]);
                    ps.setString(6,arr[2]);
                    ps.setString(7,arr[3]);
                    ps.setString(8,arr[4]);
                    ps.setString(9,planId);
                    ps.execute();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(reader1!= null){
                reader1.close();
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
    public void importWCOutputResult(File file, String planId) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps1 = null;
        PreparedStatement ps2 = null;
        BufferedReader reader1 = null;
        try {
            connection = basicDataSource.getConnection();
            String sql1 = "update yqmx_output_计算结果 set WC_TEMPE = ?  where VEL_TIME = ? and VEL_L_CODE = ? and STEP_ID = ?";
            String sql2 = "update yqmx_output_计算结果 set WC_CONC = ?  where VEL_TIME = ? and VEL_L_CODE = ? and STEP_ID = ?";
            ps1 = connection.prepareStatement(sql1);
            ps2 = connection.prepareStatement(sql2);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line ;
            int count = 0;
            List<Integer> countList = new ArrayList<Integer>();
            List<String> firstData = new ArrayList<String>();

            while ((line = reader.readLine()) != null){
                count++;
                String newLine = line.replaceAll("\\s{1,}", " ");
                if(newLine.contains("TIME")){
                    String[] arr = newLine.split(" ");
                    firstData.add(arr[2]);
                    countList.add(count);
                }
            }
            reader1 = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line1 ;
            int rowIndex = 0;
            while ((line1 = reader1.readLine()) != null){
                rowIndex++;
                for(int i = 0 ; i < countList.size()-1 ; i++){
                    if(rowIndex>countList.get(i) && rowIndex<countList.get(i+1)){
                        String newLine = line1.replaceAll("\\s{1,}", " ");
                        String[] arr = newLine.split(" ");
                        //节点是偶数时，奇数是温度，偶数是染料浓度
                        //节点是奇数时，偶数是温度，奇数是染料浓度
                        if(countList.get(i) % 2 == 0){
                            if(rowIndex % 2 == 0){
                                ps2.setString(1,arr[2]);
                                ps2.setString(2,firstData.get(i));
                                ps2.setString(3,arr[1]);
                                ps2.setString(4,planId);
                                ps2.execute();
                            }else{
                                ps1.setString(1,arr[2]);
                                ps1.setString(2,firstData.get(i));
                                ps1.setString(3,arr[1]);
                                ps1.setString(4,planId);
                                ps1.execute();
                            }
                        }else{
                            if(rowIndex % 2 == 0){
                                ps1.setString(1,arr[2]);
                                ps1.setString(2,firstData.get(i));
                                ps1.setString(3,arr[1]);
                                ps1.setString(4,planId);
                                ps1.execute();
                            }else{
                                ps2.setString(1,arr[2]);
                                ps2.setString(2,firstData.get(i));
                                ps2.setString(3,arr[1]);
                                ps2.setString(4,planId);
                                ps2.execute();
                            }
                        }

                    }
                }
                if(rowIndex>countList.get(countList.size()-1)){
                    String newLine = line1.replaceAll("\\s{1,}", " ");
                    String[] arr = newLine.split(" ");
                    //节点是偶数时，奇数是温度，偶数是染料浓度
                    //节点是奇数时，偶数是温度，奇数是染料浓度
                    if(countList.get(countList.size()-1) % 2 == 0){
                        if(rowIndex % 2 == 0){
                            ps2.setString(1,arr[2]);
                            ps2.setString(2,firstData.get(countList.size()-1));
                            ps2.setString(3,arr[1]);
                            ps2.setString(4,planId);
                            ps2.execute();
                        }else{
                            ps1.setString(1,arr[2]);
                            ps1.setString(2,firstData.get(countList.size()-1));
                            ps1.setString(3,arr[1]);
                            ps1.setString(4,planId);
                            ps1.execute();
                        }
                    }else{
                        if(rowIndex % 2 == 0){
                            ps1.setString(1,arr[2]);
                            ps1.setString(2,firstData.get(countList.size()-1));
                            ps1.setString(3,arr[1]);
                            ps1.setString(4,planId);
                            ps1.execute();
                        }else{
                            ps2.setString(1,arr[2]);
                            ps2.setString(2,firstData.get(countList.size()-1));
                            ps2.setString(3,arr[1]);
                            ps2.setString(4,planId);
                            ps2.execute();
                        }
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(reader1 != null){
                reader1.close();
            }
            if(ps1 != null){
                ps1.close();
            }
            if(ps2 != null){
                ps2.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public void importWSOutputResult(File file, String planId) throws SQLException, IOException {
        Connection connection = null;
        PreparedStatement ps1 = null;
        BufferedReader reader = null;
        BufferedReader reader1 = null;
        try {

            connection = basicDataSource.getConnection();
            String sql1 = "update yqmx_output_计算结果 set WS_DEPTH = ?  where VEL_TIME = ? and VEL_L_CODE = ? and step_id = ?";
            ps1 = connection.prepareStatement(sql1);
             reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line ;
            int count = 0;
            List<Integer> countList = new ArrayList<Integer>();
            List<String> firstData = new ArrayList<String>();
//            List<Object> secondData = new ArrayList<Object>();

            while ((line = reader.readLine()) != null){
                count++;
                String newLine = line.replaceAll("\\s{1,}", " ");
                if(newLine.contains("TIME")){
                    String[] arr = newLine.split(" ");
                    firstData.add(arr[4]);
                    countList.add(count);
                }
            }
             reader1 = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line1 ;
            int rowIndex = 0;
            while ((line1 = reader1.readLine()) != null){
                rowIndex++;
                for(int i = 0 ; i < countList.size()-1 ; i++){
                    if(rowIndex>countList.get(i) && rowIndex<countList.get(i+1)){
                        String newLine = line1.replaceAll("\\s{1,}", " ");
                        String[] arr = newLine.split(" ");
                        ps1.setString(1,arr[2]);
                        ps1.setString(2,firstData.get(i));
                        ps1.setString(3,arr[1]);
                        ps1.setString(4,planId);
                        ps1.execute();
                    }
                }
                if(rowIndex>countList.get(countList.size()-1)){
                    String newLine = line1.replaceAll("\\s{1,}", " ");
                    String[] arr = newLine.split(" ");
                    ps1.setString(1,arr[2]);
                    ps1.setString(2,firstData.get(countList.size()-1));
                    ps1.setString(3,arr[1]);
                    ps1.setString(4,planId);
                    ps1.execute();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(reader!=null){
                reader.close();
            }
            if(reader1!=null){
                reader1.close();
            }
            if(ps1 != null){
                ps1.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    @Override
    public void deleteResult(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            String sql = "delete from yqmx_output_计算结果 where step_id = ?";
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(ps != null){
                ps.close();
            }
            if(connection != null){
                connection.close();
            }
        }
    }

    public Map<String,Object> getPlan(Connection connection,String planId) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        String sql = "select t.plan_id, t.方案名称, t.计算年份\n" +
                "  from sdlm_biz_方案定义表 t\n" +
                " where t.plan_id = ?";
        try {
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs = ps.executeQuery();
            if (rs.next()){
                resultMap.put("方案_id",rs.getString(1));
                resultMap.put("方案名称",rs.getString(2));
                resultMap.put("year",rs.getInt(3));
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
        return resultMap;
    }

    public List<Map<String,Object>> getOrder(Connection connection,String planId) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String,Object>>();
        try {
            String sql = " select t1.ll_xulie, t1.wd_xulie, t1.wrw_xulie, t2.l_name\n" +
                    "   from YQMX_BIZ_主控序列对应表 t1, yqmx_sys_网格信息表 t2\n" +
                    "  where t1.plan_id = ?\n" +
                    "    and t1.i_code = t2.i_code\n" +
                    "    and t1.j_code = t2.j_code\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> map = new LinkedHashMap<String,Object>();
                map.put("flowId",rs.getString(1));
                map.put("temperId",rs.getString(2));
                map.put("pollutionId",rs.getString(3));
                map.put("orderName",rs.getString(4));
                resultList.add(map);
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
        return  resultList;
    }

    public Map<String, Object> getBoundaryConditions(String planId,String fileName) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        try {
            connection = basicDataSource.getConnection();
            List<Map<String,Object>> list = getOrder(connection,planId);
            Map<String,Object> plan = getPlan(connection,planId);
            String sql = "select distinct day, 参数_value\n" +
                    "  from yqmx_biz_运算参数表\n" +
                    " where 方案_NO = ?\n" +
                    "   and 序列_ID = ?\n" +
                    "   and 参数项目_id in\n" +
                    "       (select 参数项目_id\n" +
                    "          from sdlm_sys_运算参数定义表\n" +
                    "         where file_id = (select file_id\n" +
                    "                            from sdlm_sys_参数文件\n" +
                    "                           where file_name = ?))\n" +
                    " order by day\n";
            ps = connection.prepareStatement(sql);
            List<Map<String,Object>> orderList = new ArrayList<Map<String, Object>>();
            for(int i = 0 ; i < list.size() ; i++){
                Map<String,Object> map = list.get(i);
                Map<String,Object> orderMap = new LinkedHashMap<String,Object>();
                String id = null;
                if("qser.inp".equals(fileName)){
                    id = (String) map.get("flowId");
                }
                if("dser.inp".equals(fileName)){
                    id = (String) map.get("pollutionId");
                }
                if("tser.inp".equals(fileName)){
                    id = (String) map.get("temperId");
                }
                orderMap.put("序列_id",id);
                orderMap.put("序列名称",map.get("orderName"));
                ps.setString(1,planId);
                ps.setString(2,id);
                ps.setString(3,fileName);
                rs = ps.executeQuery();
                List<Map<String,Object>> dataList = new ArrayList<Map<String, Object>>();
                Calendar calendar = Calendar.getInstance();
                calendar.set((Integer) plan.get("year"),0,0);
                int startDay = -1;
                while (rs.next()){
                    Map<String,Object> dataMap = new LinkedHashMap<String,Object>();
                    calendar.add(Calendar.DAY_OF_MONTH,rs.getInt(1)-startDay);
                    startDay = rs.getInt(1);
                    dataMap.put("天",sdf.format(calendar.getTime()));//rs.getInt(1)
                    if("qser.inp".equals(fileName)){
                        dataMap.put("流量",rs.getDouble(2));
                    }
                    if("dser.inp".equals(fileName)){
                        dataMap.put("浓度",rs.getDouble(2));
                    }
                    if("tser.inp".equals(fileName)){
                        dataMap.put("温度",rs.getDouble(2));
                    }
                    dataList.add(dataMap);
                }
                orderMap.put("data",dataList);
                orderList.add(orderMap);
            }
            resultMap.putAll(getPlan(connection,planId));
            resultMap.put("info",orderList);
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

    @Override
    public Map<String, Object> getFlowBoundaryConditions(String planId) throws SQLException {
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            resultMap = getBoundaryConditions(planId,"qser.inp");
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> getPollutionBoundaryConditions(String planId) throws SQLException {
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            resultMap = getBoundaryConditions(planId,"dser.inp");
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> getTemperatureBoundaryConditions(String planId) throws SQLException {
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            resultMap = getBoundaryConditions(planId,"tser.inp");
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
        return resultMap;
    }

    public Map<String,Object> getCurve(Connection connection ,String planId, String iCode, String jCode , String type) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        try {
            String sql = "select t2.l_code,\n" +
                    "       t2.i_code,\n" +
                    "       t2.j_code,\n" +
                    "       t1.vel_n,\n" +
                    "       t1.vel_time,\n" +
                    "       t1.ws_depth,\n" +
                    "       t1.wc_tempe,\n" +
                    "       t1.wc_conc\n" +
                    "  from yqmx_output_计算结果 t1, yqmx_sys_网格信息表 t2\n" +
                    " where t1.vel_l_code = t2.l_code\n" +
                    "   and t1.step_id = ?\n" +
                    "   and t2.i_code = ?\n" +
                    "   and t2.j_code = ?\n";
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.setString(2,iCode);
            ps.setString(3,jCode);
            rs = ps.executeQuery();
            List<Map<String,Object>> tempList = new ArrayList<Map<String,Object>>();
            String l_code = null;
            String i_code = null;
            String j_code = null;
            while (rs.next()){
                Map<String,Object> tempMap = new LinkedHashMap<String,Object>();
                if(l_code == null){
                    resultMap.put("L_code",rs.getString(1));
                }
                if(i_code == null){
                    resultMap.put("i_code",rs.getString(2));
                }
                if(j_code == null){
                    resultMap.put("j_code",rs.getString(3));
                }
                tempMap.put("n",rs.getString(4));
                tempMap.put("time",rs.getDouble(5));
                if("depth".equals(type)){
                    tempMap.put("depth",rs.getDouble(6));
                }
                if("tempe".equals(type)){
                    tempMap.put("tempe",rs.getDouble(7));
                }
                if("conc".equals(type)){
                    tempMap.put("conc",rs.getDouble(8));
                }
                tempList.add(tempMap);
            }
            MapSortUtil.asc2(tempList,"time","double");
            Calendar calendar = Calendar.getInstance();
            Map<String,Object> plan = getPlan(connection,planId);
            calendar.set((Integer) plan.get("year"),0,0);
            double startDay = -1;
            for(int i = 0 ; i < tempList.size() ; i++){
                Map<String,Object> map = tempList.get(i);
                double day = (double) map.get("time");
                calendar.add(Calendar.DAY_OF_MONTH,(int)(day - startDay));
                map.put("time",sdf.format(calendar.getTime()));
                startDay = day;
            }
            resultMap.put("data",tempList);
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
        return resultMap;
    }

    @Override
    public Map<String, Object> getDepthCurve(String planId, String iCode, String jCode) throws SQLException {
        Connection connection = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            resultMap.putAll(getPlan(connection,planId));
            resultMap.put("info",getCurve(connection,planId,iCode,jCode,"depth"));
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(connection != null){
                connection.close();
            }
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> getTemperatureCurve(String planId, String iCode, String jCode) throws SQLException {
        Connection connection = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            resultMap.putAll(getPlan(connection,planId));
            resultMap.put("info",getCurve(connection,planId,iCode,jCode,"tempe"));
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(connection != null){
                connection.close();
            }
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> getPollutionCurve(String planId, String iCode, String jCode) throws SQLException {
        Connection connection = null;
        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
        try {
            connection = basicDataSource.getConnection();
            resultMap.putAll(getPlan(connection,planId));
            resultMap.put("info",getCurve(connection,planId,iCode,jCode,"conc"));
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }finally {
            if(connection != null){
                connection.close();
            }
        }
        return resultMap;
    }

    public List<Map<String,Object>> getColorMap(Connection connection , String type) throws SQLException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String,Object>> resultList = new ArrayList<Map<String, Object>>();
        try {
            String sql = "select t.level_min_value, t.level_max_value, t.level_decs, t.color_value\n" +
                    "  from yqmx_sys_地图颜色定义 t\n" +
                    " where t.type_id = ?\n" +
                    " order by t.level_id\n";
            ps = connection.prepareStatement(sql);
            if("depth".equals(type)){
                ps.setString(1,"2");
            }
            if("temperature".equals(type)){
                ps.setString(1,"3");
            }
            if("concentration".equals(type)){
                ps.setString(1,"1");
            }
            rs = ps.executeQuery();
            while (rs.next()){
                Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
                resultMap.put("minValue", StringUtils.isNotEmpty(rs.getString(1))?rs.getDouble(1):null);
                resultMap.put("maxValue",StringUtils.isNotEmpty(rs.getString(2))?rs.getDouble(2):null);
                resultMap.put("desc",rs.getString(3));
                resultMap.put("colorValue",rs.getString(4));
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
        }
        return resultList;
    }

//    public Map<String,Object> renderResult(Connection connection , String planId , String type) throws SQLException {
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//        Map<String,Object> resultMap = new LinkedHashMap<String,Object>();
//        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//        try {
//            String sql1 = "select distinct t.vel_n, t.vel_time\n" +
//                    "  from yqmx_output_计算结果 t\n" +
//                    " where t.step_id = ?\n" +
//                    " order by t.vel_n\n";
//            String sql2 = "select t2.l_code,\n" +
//                    "       t2.i_code,\n" +
//                    "       t2.j_code,\n" +
//                    "       t2.经度,\n" +
//                    "       t2.纬度,\n" +
//                    "       t2.super_obj_id,\n" +
//                    "       t1.ws_depth,\n" +
//                    "       t1.wc_tempe,\n" +
//                    "       t1.wc_conc\n" +
//                    "  from yqmx_output_计算结果 t1, yqmx_sys_网格信息表 t2\n" +
//                    " where t1.vel_l_code = t2.l_code\n" +
//                    "   and t1.step_id = ?\n" +
//                    "   and t1.vel_n = ?\n" +
//                    "   and t1.vel_time = ?\n";
//            ps = connection.prepareStatement(sql1);
//            ps.setString(1,planId);
//            rs = ps.executeQuery();
//            List<Map<String,Object>> timeList = new ArrayList<Map<String,Object>>();
//            while (rs.next()){
//                Map<String,Object> timeMap = new HashMap<String,Object>();
//                timeMap.put("n",rs.getString(1));
//                timeMap.put("time",rs.getDouble(2));
//                timeList.add(timeMap);
//            }
//            MapSortUtil.asc2(timeList,"time","double");
//            rs = null;
//            ps = null;
//
//            List<Map<String,Object>> colorMap = getColorMap(connection,type);
//
//            ps = connection.prepareStatement(sql2);
//            List<Map<String,Object>> tempList = new ArrayList<Map<String, Object>>();
//            for(int i = 0 ; i < timeList.size() ; i++){
//                Map<String,Object> timeMap = timeList.get(i);
//                String n = (String) timeMap.get("n");
//                double time = (double) timeMap.get("time");
//                ps.setString(1,planId);
//                ps.setString(2,n);
//                ps.setDouble(3,time);
//                rs = ps.executeQuery();
//                List<Map<String,Object>> list = new ArrayList<Map<String, Object>>();
//                while (rs.next()){
//                    Map<String,Object> map = new LinkedHashMap<String,Object>();
//                    map.put("L_code",rs.getString(1));
//                    map.put("i_code",rs.getString(2));
//                    map.put("j_code",rs.getString(3));
//                    map.put("coordinate",rs.getString(4)+","+rs.getString(5));
//                    map.put("super_obj_id",rs.getString(6));
//                    if("depth".equals(type)){
//                        map.put("水深",rs.getDouble(7));
//                        for(int j = 0 ; j < colorMap.size() ; j++){
//                            Map<String,Object> color = colorMap.get(j);
//                            double minValue = (double) color.get("minValue");
//                            double maxValue = (double) color.get("maxValue");
//                            if(rs.getDouble(7) >= minValue && rs.getDouble(7) < maxValue){
//                                map.put("颜色值",color.get("colorValue"));
//                            }
//                        }
//                    }
//                    if("temperature".equals(type)){
//                        map.put("温度",rs.getDouble(8));
//                        for(int j = 0 ; j < colorMap.size() ; j++){
//                            Map<String,Object> color = colorMap.get(j);
//                            if(color.get("minValue") == null ){
//                                double maxValue = (double) color.get("maxValue");
//                                if(rs.getDouble(8) < maxValue){
//                                    map.put("颜色值",color.get("colorValue"));
//                                }
//                            }
//                            if(color.get("minValue") != null && color.get("maxValue") != null){
//                                double minValue = (double) color.get("minValue");
//                                double maxValue = (double) color.get("maxValue");
//                                if(rs.getDouble(8) >= minValue && rs.getDouble(8) < maxValue){
//                                    map.put("颜色值",color.get("colorValue"));
//                                }
//                            }
//                            if(color.get("maxValue") == null){
//                                double minValue = (double) color.get("minValue");
//                                if(rs.getDouble(8) > minValue){
//                                    map.put("颜色值",color.get("colorValue"));
//                                }
//                            }
//
//                        }
//                    }
//                    if("concentration".equals(type)){
//                        map.put("浓度",rs.getDouble(9));
//                        for(int j = 0 ; j < colorMap.size() ; j++){
//                            Map<String,Object> color = colorMap.get(j);
//                            double minValue = (double) color.get("minValue");
//                            double maxValue = (double) color.get("maxValue");
//                            if(rs.getDouble(9) >= minValue && rs.getDouble(9) < maxValue){
//                                map.put("颜色值",color.get("colorValue"));
//                            }
//                        }
//                    }
//                    list.add(map);
//                }
//                timeMap.put("data",list);
//                tempList.add(timeMap);
//            }
//            Map<String,Object> plan = getPlan(connection,planId);
//            Calendar calendar = Calendar.getInstance();
//            calendar.set((Integer) plan.get("year"),0,0);
//            double starDay = -1;
//            for(int i = 0 ; i < tempList.size() ; i++){
//                Map<String,Object> map = tempList.get(i);
//                double day = (double) map.get("time");
//                calendar.add(Calendar.DAY_OF_MONTH,(int)(day - starDay));
//                starDay = day;
//                map.put("time",sdf.format(calendar.getTime()));
//            }
//            List<Map<String,String>> legendList = new ArrayList<Map<String,String>>();
//            for(int k = 0 ; k < colorMap.size() ; k++){
//                Map<String,Object> map = colorMap.get(k);
//                Map<String,String> legendMap = new LinkedHashMap<String,String>();
//                legendMap.put("desc", (String) map.get("desc"));
//                legendMap.put("colorValue", (String) map.get("colorValue"));
//                legendList.add(legendMap);
//            }
//            resultMap.put("info",tempList);
//            resultMap.put("图例定义",legendList);
//        } catch (SQLException e) {
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
//        return resultMap;
//    }

    public Map<String,Object> renderResult(Connection connection , String planId , String type , int month) throws SQLException, ParseException {
        PreparedStatement ps = null;
        ResultSet rs = null;
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Map<String,Object> resultMap = new HashMap<String,Object>();

        String sql2 = " select CAST(t1.vel_time AS NUMBER(5)) as vel_time,\n" +
                "        t1.vel_n,\n" +
                "        t2.l_code,\n" +
                "        t2.i_code,\n" +
                "        t2.j_code,\n" +
                "        t2.经度,\n" +
                "        t2.纬度,\n" +
                "        t2.super_obj_id,\n" +
                "        t1.ws_depth,\n" +
                "        t1.wc_tempe,\n" +
                "        t1.wc_conc\n" +
                "   from yqmx_output_计算结果 t1, yqmx_sys_网格信息表 t2\n" +
                "  where t1.vel_l_code = t2.l_code\n" +
                "    and t1.step_id = ?\n" +
                "    and t1.vel_time <= ?\n" +
                "    and t1.vel_time >= ?\n" +
                "  order by CAST(t1.vel_time AS NUMBER(5))";
        try{

            List<Map<String,Object>> colorMap = getColorMap(connection,type);


            //存放一年的信息
            List<Map<String,Object>> list = new ArrayList<Map<String, Object>>();
            //存放每一天的信息
            Map<String,Object> map = null;
            //存放每一天中的data
            List<Map<String,Object>> dataList = null;
            //存放每一天中的dataMap
            Map<String,Object> dataMap = null;
            Map<String,Object> plan = getPlan(connection,planId);

            int count = 0;
            Calendar instance1 = Calendar.getInstance();
            Date date1 = sdf.parse(plan.get("year")+"-"+month+"-01");
            instance1.setTime(date1);
            int start = instance1.get(Calendar.DAY_OF_YEAR);

            Calendar instance = Calendar.getInstance();
            Date date = sdf.parse(plan.get("year")+"-"+(month+1)+"-01");
            instance.setTime(date);
            int end = instance.get(Calendar.DAY_OF_YEAR);

            Calendar calendar = Calendar.getInstance();
            calendar.set((Integer) plan.get("year"),0,0);
            ps = connection.prepareStatement(sql2);
            ps.setString(1,planId);
            int year = (Integer) plan.get("year");
            int dayFlag = 0;
            if(month == 12 ){
                if(year % 4 == 0){
                    ps.setInt(2,365);
                    dayFlag = 365;
                }else{
                    ps.setInt(2,364);
                    dayFlag = 364;
                }
            }else{
                ps.setInt(2,end-2);
                dayFlag = end - 2;
            }
            ps.setInt(3,start-1);
            rs = ps.executeQuery();
            double startDay = 0;

            while (rs.next()){
                int day = rs.getInt(1);

                if(map == null){
                    map = new LinkedHashMap<String,Object>();
                    calendar.add(Calendar.DAY_OF_MONTH,day+1);
                    startDay = day;
                    map.put("time",sdf.format(calendar.getTime()));
//                    map.put("n",rs.getString(2));
//                    dataList = new ArrayList<Map<String,Object>>();
                    dataMap = new LinkedHashMap<String,Object>();
//                    dataMap.put("L_code",rs.getString(3));
//                    dataMap.put("i_code",rs.getString(4));
//                    dataMap.put("j_code",rs.getString(5));
//                    dataMap.put("coordinate",rs.getString(6)+","+rs.getString(7));
//                    dataMap.put("super_obj_id",rs.getString(8));

                    Map<String,Object> idMap = new LinkedHashMap<String,Object>();
                    if("depth".equals(type)){
                        idMap.put("d",rs.getDouble(9));
                        for(int j = 0 ; j < colorMap.size() ; j++){
                            Map<String,Object> color = colorMap.get(j);
                            double minValue = (double) color.get("minValue");
                            double maxValue = (double) color.get("maxValue");
                            if(rs.getDouble(9) >= minValue && rs.getDouble(9) < maxValue){
//                                dataMap.put("颜色值",color.get("colorValue"));
                                idMap.put("r",color.get("colorValue"));
                            }
                        }
                    }
                    if("temperature".equals(type)){
                        idMap.put("t",rs.getDouble(10));
                        for(int j = 0 ; j < colorMap.size() ; j++){
                            Map<String,Object> color = colorMap.get(j);
                            if(color.get("minValue") == null ){
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(10) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                            if(color.get("minValue") != null && color.get("maxValue") != null){
                                double minValue = (double) color.get("minValue");
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(10) >= minValue && rs.getDouble(10) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                            if(color.get("maxValue") == null){
                                double minValue = (double) color.get("minValue");
                                if(rs.getDouble(10) > minValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }

                        }
                    }
                    if("concentration".equals(type)){
                        idMap.put("c",rs.getDouble(11));
                        for(int j = 0 ; j < colorMap.size() ; j++){
                            Map<String,Object> color = colorMap.get(j);
                            double minValue = (double) color.get("minValue");
                            double maxValue = (double) color.get("maxValue");
                            if(rs.getDouble(11) >= minValue && rs.getDouble(11) < maxValue){
//                                dataMap.put("颜色值",color.get("colorValue"));
                                idMap.put("r",color.get("colorValue"));
                            }
                        }
                    }
                    if(dayFlag == day){
                        count++;
                    }
                    dataMap.put(rs.getString(8),idMap);
                }else{
                    if(day == startDay){
//                        dataMap = new LinkedHashMap<String,Object>();
//                        dataMap.put("L_code",rs.getString(3));
//                        dataMap.put("i_code",rs.getString(4));
//                        dataMap.put("j_code",rs.getString(5));
//                        dataMap.put("coordinate",rs.getString(6)+","+rs.getString(7));
//                        dataMap.put("super_obj_id",rs.getString(8));
                        Map<String,Object> idMap = new LinkedHashMap<String,Object>();
                        if("depth".equals(type)){
                            idMap.put("d",rs.getDouble(9));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                double minValue = (double) color.get("minValue");
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(9) >= minValue && rs.getDouble(9) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                        }
                        if("temperature".equals(type)){
                            idMap.put("t",rs.getDouble(10));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                if(color.get("minValue") == null ){
                                    double maxValue = (double) color.get("maxValue");
                                    if(rs.getDouble(10) < maxValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }
                                if(color.get("minValue") != null && color.get("maxValue") != null){
                                    double minValue = (double) color.get("minValue");
                                    double maxValue = (double) color.get("maxValue");
                                    if(rs.getDouble(10) >= minValue && rs.getDouble(10) < maxValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }
                                if(color.get("maxValue") == null){
                                    double minValue = (double) color.get("minValue");
                                    if(rs.getDouble(10) > minValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }

                            }
                        }
                        if("concentration".equals(type)){
                            idMap.put("c",rs.getDouble(11));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                double minValue = (double) color.get("minValue");
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(11) >= minValue && rs.getDouble(11) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                        }
//                        dataList.add(dataMap);
                        dataMap.put(rs.getString(8),idMap);
                        if(dayFlag == day){
                            count++;
                        }
                        if(dayFlag == day && count == 1926){
                            map.put("data",dataMap);
                            list.add(map);
                        }
                    }else{
                        map.put("data",dataMap);
                        list.add(map);
                        map = new LinkedHashMap<String,Object>();
                        calendar.add(Calendar.DAY_OF_MONTH,(int)(day - startDay));
                        startDay = day;
                        map.put("time",sdf.format(calendar.getTime()));
                        map.put("n",rs.getString(2));
//                        dataList = new ArrayList<Map<String,Object>>();
                        dataMap = new LinkedHashMap<String,Object>();
//                        dataMap.put("L_code",rs.getString(3));
//                        dataMap.put("i_code",rs.getString(4));
//                        dataMap.put("j_code",rs.getString(5));
//                        dataMap.put("coordinate",rs.getString(6)+","+rs.getString(7));
//                        dataMap.put("super_obj_id",rs.getString(8));
                        Map<String,Object> idMap = new LinkedHashMap<String,Object>();
                        if("depth".equals(type)){
                            idMap.put("d",rs.getDouble(9));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                double minValue = (double) color.get("minValue");
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(9) >= minValue && rs.getDouble(9) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                        }
                        if("temperature".equals(type)){
                            idMap.put("t",rs.getDouble(10));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                if(color.get("minValue") == null ){
                                    double maxValue = (double) color.get("maxValue");
                                    if(rs.getDouble(10) < maxValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }
                                if(color.get("minValue") != null && color.get("maxValue") != null){
                                    double minValue = (double) color.get("minValue");
                                    double maxValue = (double) color.get("maxValue");
                                    if(rs.getDouble(10) >= minValue && rs.getDouble(10) < maxValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }
                                if(color.get("maxValue") == null){
                                    double minValue = (double) color.get("minValue");
                                    if(rs.getDouble(10) > minValue){
//                                        dataMap.put("颜色值",color.get("colorValue"));
                                        idMap.put("r",color.get("colorValue"));
                                    }
                                }

                            }
                        }
                        if("concentration".equals(type)){
                            idMap.put("c",rs.getDouble(11));
                            for(int j = 0 ; j < colorMap.size() ; j++){
                                Map<String,Object> color = colorMap.get(j);
                                double minValue = (double) color.get("minValue");
                                double maxValue = (double) color.get("maxValue");
                                if(rs.getDouble(11) >= minValue && rs.getDouble(11) < maxValue){
//                                    dataMap.put("颜色值",color.get("colorValue"));
                                    idMap.put("r",color.get("colorValue"));
                                }
                            }
                        }
                        if(dayFlag == day){
                            count++;
                        }
//                        dataList.add(dataMap);
                        dataMap.put(rs.getString(8),idMap);
                    }
                }
            }
            List<Map<String,String>> legendList = new ArrayList<Map<String,String>>();
            for(int k = 0 ; k < colorMap.size() ; k++){
                Map<String,Object> map1 = colorMap.get(k);
                Map<String,String> legendMap = new LinkedHashMap<String,String>();
                legendMap.put("desc", (String) map1.get("desc"));
                legendMap.put("colorValue", (String) map1.get("colorValue"));
                legendList.add(legendMap);
            }
            resultMap.put("info",list);
            resultMap.put("图例定义",legendList);
        }catch (Exception e) {
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
        return resultMap;
    }

    @Override
    public Map<String, Object> renderDepthResult(String planId,int month) throws SQLException {
        Connection connection = null;
        Map<String, Object> resultMap = null;
        Object obj= CacheUtil.get("yuQiaoResult", "renderDepthResult_"+planId+"_"+month);
        if(obj == null){
            resultMap = new LinkedHashMap<String,Object>();
            try {
                connection = basicDataSource.getConnection();
                resultMap.putAll(getPlan(connection,planId));
                resultMap.put("month",month);
                resultMap.putAll(renderResult(connection,planId,"depth",month));
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (ParseException e) {
                e.printStackTrace();
            } finally {
                if(connection != null){
                    connection.close();
                }
            }
            CacheUtil.save("yuQiaoResult","renderDepthResult_"+planId+"_"+month,resultMap);
        }else{
            resultMap = (Map<String, Object>) obj;
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> renderTemperatureResult(String planId , int month) throws SQLException {
        Connection connection = null;
        Map<String, Object> resultMap = null;
        Object obj= CacheUtil.get("yuQiaoResult", "renderTemperatureResult_"+planId+"_"+month);
        if(obj==null){
            resultMap = new LinkedHashMap<String,Object>();
            try {
                connection = basicDataSource.getConnection();
                resultMap.putAll(getPlan(connection,planId));
                resultMap.put("month",month);
                resultMap.putAll(renderResult(connection,planId,"temperature",month));
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (ParseException e) {
                e.printStackTrace();
            } finally {
                if(connection != null){
                    connection.close();
                }
            }
            CacheUtil.save("yuQiaoResult","renderTemperatureResult_"+planId+"_"+month,resultMap);
        }else{
            resultMap = (Map<String, Object>) obj;
        }
        return resultMap;
    }

    @Override
    public Map<String, Object> renderConcentrationResult(String planId,int month) throws SQLException {
        Connection connection = null;
        Map<String, Object> resultMap = null;
        Object obj= CacheUtil.get("yuQiaoResult", "renderConcentrationResult_"+planId+"_"+month);
        if(obj == null){
            try {
                resultMap = new LinkedHashMap<String,Object>();
                connection = basicDataSource.getConnection();
                resultMap.putAll(getPlan(connection,planId));
                resultMap.put("month",month);
                resultMap.putAll(renderResult(connection,planId,"concentration",month));
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (ParseException e) {
                e.printStackTrace();
            } finally {
                if(connection != null){
                    connection.close();
                }
            }
            CacheUtil.save("yuQiaoResult", "renderConcentrationResult_"+planId+"_"+month,resultMap);
        }else{
            resultMap = (Map<String, Object>) obj;
        }

        return resultMap;
    }

//    @Override
//    public Map<String, Object> renderFlowSpeedAndDirectionResult(String planId) throws SQLException {
//        Connection connection = null;
//        PreparedStatement ps = null;
//        ResultSet rs = null;
//        Map<String, Object> resultMap = null;
//        Object obj = CacheUtil.get("yuQiaoResult", "renderFlowSpeedAndDirectionResult_"+planId);
//        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
//        if(obj == null){
//            try {
//                resultMap = new LinkedHashMap<String,Object>();
//                connection = basicDataSource.getConnection();
//                resultMap.putAll(getPlan(connection,planId));
//                String sql1 = "select distinct t.vel_n, t.vel_time\n" +
//                        "  from yqmx_output_计算结果 t\n" +
//                        " where t.step_id = ?\n" +
//                        " order by t.vel_n\n";
//                String sql2 = "   select t2.l_code,\n" +
//                        "          t2.i_code,\n" +
//                        "          t2.j_code,\n" +
//                        "          t2.经度,\n" +
//                        "          t2.纬度,\n" +
//                        "          t2.super_obj_id,\n" +
//                        "          t1.vel_x_value,\n" +
//                        "          t1.vel_y_value\n" +
//                        "     from yqmx_output_计算结果 t1, yqmx_sys_网格信息表 t2\n" +
//                        "    where t1.vel_l_code = t2.l_code\n" +
//                        "      and t1.step_id = ?\n" +
//                        "      and t1.vel_time = ?\n";
//                String sql3 = "select t.level_id, t.level_min_value, t.level_max_value\n" +
//                        "  from yqmx_sys_流速等级定义 t\n";
//                ps = connection.prepareStatement(sql1);
//                ps.setString(1,planId);
//                rs = ps.executeQuery();
//                List<Double> dayList = new ArrayList<Double>();
//                while (rs.next()){
//                    dayList.add(rs.getDouble(2));
//                }
//                Object[] dayArr =  dayList.toArray();
//                //冒泡升序排列
//                double temp = 0;
//                for (int i = 0; i < dayArr.length; i++) {
//                    for (int j = dayArr.length - 1; j > i; j--) {
//                        if ((double)dayArr[j] < (double)dayArr[j - 1]) {
//                            temp = (double)dayArr[j] ;
//                            dayArr[j] = dayArr[j - 1];
//                            dayArr[j - 1] = temp;
//                        }
//                    }
//                }
//
//                ps = null;
//                rs = null;
//
//                ps = connection.prepareStatement(sql3);
//                rs = ps.executeQuery();
//                List<Map<String,Object>> gradeList = new ArrayList<Map<String,Object>>();
//                while (rs.next()){
//                    Map<String,Object> gradeMap = new HashMap<String,Object>();
//                    gradeMap.put("levelId",rs.getInt(1));
//                    gradeMap.put("minValue",rs.getDouble(2));
//                    gradeMap.put("maxValue",rs.getDouble(3));
//                    gradeList.add(gradeMap);
//                }
//
//                ps = null;
//                rs = null;
//
//                Map<String,Object> plan = getPlan(connection,planId);
//
//                ps = connection.prepareStatement(sql2);
//                List<Map<String,Object>> infoList = new ArrayList<Map<String, Object>>();
//                Calendar calendar = Calendar.getInstance();
//                calendar.set((Integer) plan.get("year"),0,0);
//                double startDay = -1;
//                for(int i = 0 ; i < dayArr.length ; i++){
//                    double day = (double)dayArr[i];
//                    ps.setString(1,planId);
//                    ps.setDouble(2,day);
//                    rs = ps.executeQuery();
//                    Map<String,Object> infoMap = new LinkedHashMap<String,Object>();
//                    List<Map<String,Object>> dataList = new ArrayList<Map<String, Object>>();
//                    while (rs.next()){
//                        Map<String,Object> dataMap = new LinkedHashMap<String,Object>();
//                        dataMap.put("L_code",rs.getString(1));
//                        dataMap.put("i_code",rs.getString(2));
//                        dataMap.put("j_code",rs.getString(3));
//                        dataMap.put("coordinate",rs.getString(4)+","+rs.getString(5));
//                        dataMap.put("super_obj_id",rs.getString(6));
//                        dataMap.put("x_value",rs.getDouble(7));
//                        dataMap.put("y_value",rs.getDouble(8));
//                        if(rs.getDouble(7) == 0 && rs.getDouble(8) == 0){
//                            double speed = 0;
//                            dataMap.put("流速",speed);
//                            dataMap.put("流向",0);
//                            dataMap.put("流速等级",1);
//                        }else{
//                            double speed = Math.sqrt(rs.getDouble(7)*rs.getDouble(7) + rs.getDouble(8)*rs.getDouble(8));
//                            dataMap.put("流速",speed);
//
//                            for(int k = 0 ; k < gradeList.size() ; k++){
//                                Map<String,Object> map = gradeList.get(k);
//                                if(speed >= (double)map.get("minValue") && speed < (double)map.get("maxValue")){
//                                    dataMap.put("流速等级",map.get("levelId"));
//                                    break;
//                                }
//                            }
//
//                            if(rs.getDouble(7) == 0){
//                                if(rs.getDouble(8)>0){
//                                    dataMap.put("流向",90);
//                                }else{
//                                    dataMap.put("流向",270);
//                                }
//                            }else if(rs.getDouble(8) == 0){
//                                if(rs.getDouble(7)>0){
//                                    dataMap.put("流向",0);
//                                }else{
//                                    dataMap.put("流向",180);
//                                }
//                            }else{
//                                double direction = Math.abs(Math.atan(rs.getDouble(8)/rs.getDouble(7))*(180/Math.PI));
//                                if(rs.getDouble(7)>0 && rs.getDouble(8)>0){
//                                    dataMap.put("流向",direction);
//                                }else if(rs.getDouble(7)<0 && rs.getDouble(8)>0){
//                                    dataMap.put("流向",direction+90);
//                                }else if(rs.getDouble(7)<0 && rs.getDouble(8)<0){
//                                    dataMap.put("流向",direction+180);
//                                }else if(rs.getDouble(7)>0 && rs.getDouble(8)<0){
//                                    dataMap.put("流向",direction+270);
//                                }
//                            }
//                        }
//                        dataMap.put("颜色值"," #004A80");//蓝色
//                        dataList.add(dataMap);
//                    }
//                    calendar.add(Calendar.DAY_OF_MONTH,(int)(day - startDay));
//                    startDay = day;
//                    infoMap.put("天",sdf.format(calendar.getTime()));
//                    infoMap.put("data",dataList);
//                    infoList.add(infoMap);
//                }
//                resultMap.put("info",infoList);
//            } catch (SQLException e) {
//                e.printStackTrace();
//                throw e;
//            }finally {
//                if(rs != null){
//                    rs.close();
//                }
//                if(ps != null){
//                    ps.close();
//                }
//                if(connection != null){
//                    connection.close();
//                }
//            }
//            CacheUtil.save("yuQiaoResult", "renderFlowSpeedAndDirectionResult_"+planId,resultMap);
//        }else{
//            resultMap = (Map<String,Object>)obj;
//        }
//
//        return resultMap;
//    }
    @Override
    public Map<String, Object> renderFlowSpeedAndDirectionResult(String planId , int month) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String, Object> resultMap = null;
        Object obj = CacheUtil.get("yuQiaoResult", "renderFlowSpeedAndDirectionResult_"+planId+"_"+month);
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        DecimalFormat df1 = new DecimalFormat("####.#####");
        DecimalFormat df2 = new DecimalFormat("####.##");
        if(obj == null){
            try {
                resultMap = new LinkedHashMap<String,Object>();
                connection = basicDataSource.getConnection();
                resultMap.putAll(getPlan(connection,planId));

                String sql2 = "select CAST(t1.vel_time AS NUMBER(5)) as vel_time,\n" +
                        "       t2.l_code,\n" +
                        "       t2.i_code,\n" +
                        "       t2.j_code,\n" +
                        "       t2.经度,\n" +
                        "       t2.纬度,\n" +
                        "       t2.super_obj_id,\n" +
                        "       t1.vel_x_value,\n" +
                        "       t1.vel_y_value\n" +
                        "  from yqmx_output_计算结果 t1, yqmx_sys_网格信息表 t2\n" +
                        " where t1.vel_l_code = t2.l_code\n" +
                        "   and t1.step_id = ?\n" +
                        "   and t1.vel_time <= ?\n" +
                        "   and t1.vel_time >= ?\n" +
                        " order by CAST(t1.vel_time AS NUMBER(5))\n";

                String sql3 = "select t.level_id, t.level_min_value, t.level_max_value\n" +
                        "  from yqmx_sys_流速等级定义 t\n";

                ps = connection.prepareStatement(sql3);
                rs = ps.executeQuery();
                List<Map<String,Object>> gradeList = new ArrayList<Map<String,Object>>();
                while (rs.next()){
                    Map<String,Object> gradeMap = new HashMap<String,Object>();
                    gradeMap.put("levelId",rs.getInt(1));
                    gradeMap.put("minValue",rs.getDouble(2));
                    gradeMap.put("maxValue",rs.getDouble(3));
                    gradeList.add(gradeMap);
                }

                ps = null;
                rs = null;

                Map<String,Object> plan = getPlan(connection,planId);



                //存放一年的信息
                List<Map<String,Object>> list = new ArrayList<Map<String, Object>>();
                //存放每一天的信息
                Map<String,Object> map = null;
                //存放每一天中的data
                List<Map<String,Object>> dataList = null;
                //存放每一天中的dataMap
                Map<String,Object> dataMap = null;


                int count = 0;
                Calendar instance1 = Calendar.getInstance();
                Date date1 = sdf.parse(plan.get("year")+"-"+month+"-01");
                instance1.setTime(date1);
                int start = instance1.get(Calendar.DAY_OF_YEAR);

                Calendar instance = Calendar.getInstance();
                Date date = sdf.parse(plan.get("year")+"-"+(month+1)+"-01");
                instance.setTime(date);
                int end = instance.get(Calendar.DAY_OF_YEAR);

                Calendar calendar = Calendar.getInstance();
                calendar.set((Integer) plan.get("year"),0,0);
                ps = connection.prepareStatement(sql2);
                ps.setString(1,planId);
                int year = (Integer) plan.get("year");
                int dayFlag = 0;
                if(month == 12 ){
                    if(year % 4 == 0){
                        ps.setInt(2,365);
                        dayFlag = 365;
                    }else{
                        ps.setInt(2,364);
                        dayFlag = 364;
                    }
                }else{
                    ps.setInt(2,end-2);
                    dayFlag = end - 2;
                }
                ps.setInt(3,start-1);
                rs = ps.executeQuery();
                double startDay = 0;
                while (rs.next()){
                    int day = rs.getInt(1);
                    if(map == null){
                        map = new LinkedHashMap<String,Object>();
                        calendar.add(Calendar.DAY_OF_MONTH,day+1);
                        startDay = day;
                        map.put("time",sdf.format(calendar.getTime()));
                        dataMap = new LinkedHashMap<String,Object>();
//                        dataMap.put("L_code",rs.getString(2));
//                        dataMap.put("i_code",rs.getString(3));
//                        dataMap.put("j_code",rs.getString(4));
//                        dataMap.put("coordinate",rs.getString(5)+","+rs.getString(6));
//                        dataMap.put("super_obj_id",rs.getString(7));

                        Map<String,Object> idMap = new LinkedHashMap<String,Object>();
//                        dataMap.put("x_value",rs.getDouble(8));
//                        dataMap.put("y_value",rs.getDouble(9));
                        if(rs.getDouble(8) == 0 && rs.getDouble(9) == 0){
                            double speed = 0;
//                            dataMap.put("流速",speed);
//                            dataMap.put("流向",0);
//                            dataMap.put("流速等级",1);
                            idMap.put("v",speed);
                            idMap.put("f",0);
                            idMap.put("l",1);
                        }else{
                            double speed = Math.sqrt(rs.getDouble(8)*rs.getDouble(8) + rs.getDouble(9)*rs.getDouble(9));
//                            dataMap.put("流速",speed);
                            idMap.put("v",Double.valueOf(df1.format(speed)));

                            for(int k = 0 ; k < gradeList.size() ; k++){
                                Map<String,Object> map1 = gradeList.get(k);
                                if(speed >= (double)map1.get("minValue") && speed < (double)map1.get("maxValue")){
//                                    dataMap.put("流速等级",1);
                                    idMap.put("l",map1.get("levelId"));
                                    break;
                                }
                            }

                            if(rs.getDouble(8) == 0){
                                if(rs.getDouble(9)>0){
//                            dataMap.put("流向",0);
                                    idMap.put("f",90);
                                }else{
 //                            dataMap.put("流向",0);
                                    idMap.put("f",270);
                                }
                            }else if(rs.getDouble(9) == 0){
                                if(rs.getDouble(8)>0){
//                            dataMap.put("流向",0);
                                    idMap.put("f",0);
                                }else{
//                            dataMap.put("流向",0);
                                    idMap.put("f",180);
                                }
                            }else{
                                double direction = Math.abs(Math.atan(rs.getDouble(9)/rs.getDouble(8))*(180/Math.PI));
                                if(rs.getDouble(8)>0 && rs.getDouble(9)>0){
//                                    dataMap.put("流向",direction);
                                    idMap.put("f",Double.valueOf(df2.format(direction)));
                                }else if(rs.getDouble(8)<0 && rs.getDouble(9)>0){
//                                    dataMap.put("流向",direction+90);
                                    idMap.put("f",Double.valueOf(df2.format(direction))+90);
                                }else if(rs.getDouble(8)<0 && rs.getDouble(9)<0){
//                                    dataMap.put("流向",direction+180);
                                    idMap.put("f",Double.valueOf(df2.format(direction))+180);
                                }else if(rs.getDouble(8)>0 && rs.getDouble(9)<0){
//                                    dataMap.put("流向",direction+270);
                                    idMap.put("f",Double.valueOf(df2.format(direction))+270);
                                }
                            }
                        }
//                        dataMap.put("颜色值"," #004A80");//蓝色
                        idMap.put("c","#004A80");//蓝色
                        dataMap.put(rs.getString(7),idMap);
//                        dataList.add(dataMap);
                        if(dayFlag == day){
                            count++;
                        }
                    }else{
                        if(day == startDay){
//                            dataMap = new LinkedHashMap<String,Object>();
//                            dataMap.put("L_code",rs.getString(2));
//                            dataMap.put("i_code",rs.getString(3));
//                            dataMap.put("j_code",rs.getString(4));
//                            dataMap.put("coordinate",rs.getString(5)+","+rs.getString(6));
//                            dataMap.put("super_obj_id",rs.getString(7));
//                            dataMap.put("id",rs.getString(7));
//                            dataMap.put("x_value",rs.getDouble(8));
//                            dataMap.put("y_value",rs.getDouble(9));
                            Map<String,Object> idMap = new LinkedHashMap<String,Object>();
                            if(rs.getDouble(8) == 0 && rs.getDouble(9) == 0){
                                double speed = 0;
//                                dataMap.put("流速",speed);
//                                dataMap.put("流向",0);
//                                dataMap.put("流速等级",1);
                                idMap.put("v",speed);
                                idMap.put("f",0);
                                idMap.put("l",1);
                            }else{
                                double speed = Math.sqrt(rs.getDouble(8)*rs.getDouble(8) + rs.getDouble(9)*rs.getDouble(9));
//                                dataMap.put("流速",speed);
                                idMap.put("v",Double.valueOf(df1.format(speed)));

                                for(int k = 0 ; k < gradeList.size() ; k++){
                                    Map<String,Object> map1 = gradeList.get(k);
                                    if(speed >= (double)map1.get("minValue") && speed < (double)map1.get("maxValue")){
//                                        dataMap.put("流速等级",map1.get("levelId"));
                                        idMap.put("l",map1.get("levelId"));
                                        break;
                                    }
                                }

                                if(rs.getDouble(8) == 0){
                                    if(rs.getDouble(9)>0){
//                                        dataMap.put("流向",90);
                                        idMap.put("f",90);
                                    }else{
//                                        dataMap.put("流向",270);
                                        idMap.put("f",270);
                                    }
                                }else if(rs.getDouble(9) == 0){
                                    if(rs.getDouble(8)>0){
//                                        dataMap.put("流向",0);
                                        idMap.put("f",0);
                                    }else{
//                                        dataMap.put("流向",180);
                                        idMap.put("f",180);
                                    }
                                }else{
                                    double direction = Math.abs(Math.atan(rs.getDouble(9)/rs.getDouble(8))*(180/Math.PI));
                                    if(rs.getDouble(8)>0 && rs.getDouble(9)>0){
//                                        dataMap.put("流向",direction);
                                        idMap.put("f",Double.valueOf(df2.format(direction)));
                                    }else if(rs.getDouble(8)<0 && rs.getDouble(9)>0){
//                                        dataMap.put("流向",direction+90);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+90);
                                    }else if(rs.getDouble(8)<0 && rs.getDouble(9)<0){
//                                        dataMap.put("流向",direction+180);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+180);
                                    }else if(rs.getDouble(8)>0 && rs.getDouble(9)<0){
//                                        dataMap.put("流向",direction+270);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+270);
                                    }
                                }
                            }
//                            dataMap.put("颜色值"," #004A80");//蓝色
                            idMap.put("c","#004A80");//蓝色
                            dataMap.put(rs.getString(7),idMap);
//                            dataList.add(dataMap);
                            if(dayFlag == day){
                                count++;
                            }
                            if(dayFlag == day && count == 1926){
                                map.put("data",dataMap);
                                list.add(map);
                            }
                        }else{
                            map.put("data",dataMap);
                            list.add(map);
                            map = new LinkedHashMap<String,Object>();
                            calendar.add(Calendar.DAY_OF_MONTH,(int)(day-startDay));
                            startDay = day;
                            map.put("time",sdf.format(calendar.getTime()));
//                            dataList = new ArrayList<Map<String,Object>>();
                            dataMap = new LinkedHashMap<String,Object>();
//                            dataMap.put("L_code",rs.getString(2));
//                            dataMap.put("i_code",rs.getString(3));
//                            dataMap.put("j_code",rs.getString(4));
//                            dataMap.put("coordinate",rs.getString(5)+","+rs.getString(6));
//                            dataMap.put("super_obj_id",rs.getString(7));
//                            dataMap.put("id",rs.getString(7));
//                            dataMap.put("x_value",rs.getDouble(8));
//                            dataMap.put("y_value",rs.getDouble(9));
                            Map<String,Object> idMap = new LinkedHashMap<String,Object>();
                            if(rs.getDouble(8) == 0 && rs.getDouble(9) == 0){
                                double speed = 0;
//                                dataMap.put("流速",speed);
//                                dataMap.put("流向",0);
//                                dataMap.put("流速等级",1);
                                idMap.put("v",speed);
                                idMap.put("f",0);
                                idMap.put("l",1);
                            }else{
                                double speed = Math.sqrt(rs.getDouble(8)*rs.getDouble(8) + rs.getDouble(9)*rs.getDouble(9));
//                                dataMap.put("流速",speed);
                                idMap.put("v",Double.valueOf(df1.format(speed)));

                                for(int k = 0 ; k < gradeList.size() ; k++){
                                    Map<String,Object> map1 = gradeList.get(k);
                                    if(speed >= (double)map1.get("minValue") && speed < (double)map1.get("maxValue")){
//                                        dataMap.put("流速等级",map1.get("levelId"));
                                        idMap.put("l",map1.get("levelId"));
                                        break;
                                    }
                                }

                                if(rs.getDouble(8) == 0){
                                    if(rs.getDouble(9)>0){
//                                        dataMap.put("流向",90);
                                        idMap.put("f",90);
                                    }else{
//                                        dataMap.put("流向",270);
                                        idMap.put("f",270);
                                    }
                                }else if(rs.getDouble(9) == 0){
                                    if(rs.getDouble(8)>0){
//                                        dataMap.put("流向",0);
                                        idMap.put("f",0);
                                    }else{
//                                        dataMap.put("流向",180);
                                        idMap.put("f",180);
                                    }
                                }else{
                                    double direction = Math.abs(Math.atan(rs.getDouble(9)/rs.getDouble(8))*(180/Math.PI));
                                    if(rs.getDouble(8)>0 && rs.getDouble(9)>0){
//                                        dataMap.put("流向",direction);
                                        idMap.put("f",Double.valueOf(df2.format(direction)));
                                    }else if(rs.getDouble(8)<0 && rs.getDouble(9)>0){
//                                        dataMap.put("流向",direction+90);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+90);
                                    }else if(rs.getDouble(8)<0 && rs.getDouble(9)<0){
//                                        dataMap.put("流向",direction+180);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+180);
                                    }else if(rs.getDouble(8)>0 && rs.getDouble(9)<0){
//                                        dataMap.put("流向",direction+270);
                                        idMap.put("f",Double.valueOf(df2.format(direction))+270);
                                    }
                                }
                            }
//                            dataMap.put("颜色值"," #004A80");//蓝色
                            idMap.put("c","#004A80");//蓝色

                            if(dayFlag == day){
                                count++;
                            }
                            dataMap.put(rs.getString(7),idMap);
//                            dataList.add(dataMap);
                        }
                    }
                }
                resultMap.put("month",month);
                resultMap.put("info",list);
            } catch (SQLException e) {
                e.printStackTrace();
                throw e;
            } catch (ParseException e) {
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
            CacheUtil.save("yuQiaoResult", "renderFlowSpeedAndDirectionResult_"+planId+"_"+month,resultMap);
        }else{
            resultMap = (Map<String,Object>)obj;
        }

        return resultMap;
    }

    @Override
    public Map<String, Integer> getMonth(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,Integer> resultMap = new HashMap<String,Integer>();
        SimpleDateFormat sdf = new SimpleDateFormat("MM");
        try {
            connection = basicDataSource.getConnection();
            String sql = "select t1.value\n" +
                    "  from sdlm_biz_方案条件设定表 t1, sdlm_sys_条件项目定义表 t2\n" +
                    " where t1.step_id = ?\n" +
                    "   and t1.key = t2.key_id\n" +
                    "   and t2.key_name = '模拟天数'";
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs = ps.executeQuery();
            int days = 0;
            if(rs.next()){
                days = rs.getInt(1);
            }
            Map<String,Object> plan = getPlan(connection,planId);
            Calendar calendar = Calendar.getInstance();
            calendar.set((Integer) plan.get("year"),0,0);
            calendar.add(Calendar.DAY_OF_MONTH,days+1);
            int months = Integer.parseInt(sdf.format(calendar.getTime()));
            resultMap.put("count",months);

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
