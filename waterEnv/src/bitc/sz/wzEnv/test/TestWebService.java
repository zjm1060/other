package bitc.sz.wzEnv.test;

import bitc.sz.wzEnv.service.ExecuteExeSZPJModel;
import bitc.sz.wzEnv.util.Md5Util;
import bitc.sz.wzEnv.webservice.QueryService;
import bitc.sz.wzEnv.webservice.YuQiaoService;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import java.io.*;
import java.sql.*;
import java.util.*;

/**
 * Created by Administrator on 2016/4/1.
 */
public class TestWebService {
    ApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
    QueryService queryService = (QueryService) applicationContext.getBean("querySerivce");
    ExecuteExeSZPJModel model = (ExecuteExeSZPJModel) applicationContext.getBean("executeExeSZPJModelImpl");

    @org.junit.Test
    public void testPeriod(){
        String str = queryService.getSZPJPeriodList("928FE3EF219048C5BFEF749785476164", "", "2010", "month");
        System.out.print(str);
    }

    @org.junit.Test
    public void testRiver(){
        String str = queryService.getSZPJRiverList("928FE3EF219048C5BFEF749785476164", "2010");
        System.out.print(str);
    }

    @Test
    public void testRiverLevel(){
        String str = queryService.getSZPJRiverLevel("928FE3EF219048C5BFEF749785476164", "month", "2010-01-01~2010-12-31", "");
        System.out.print(str);
    }
//将文件的数据导入到sdlm_sys_断面分层水力参数表中
    @Test
    public void testImportData(){
        List totalList = new ArrayList();
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);
            try {
                BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\data\\乐世华\\1DZIP07test\\cond\\TaiZiSC.bth")));
                String line ;
                try {
                    while ((line = reader.readLine()) != null){
                        int space = 0;
                        for(int i = 0 ; i < line.length() ; i++){
                            while(space<=line.length()-1&&line.charAt(space)==' ')
                            {
                                space++;
                            }
                        }
                        //将本行的多个空格用一个空格替换
                        String newLine = line.replaceAll("\\s{1,}", " ");
                        //以空格分隔，生成数组
                        String[] arr = newLine.split(" ");
                        //用list存放数据
                        List<String> list = new ArrayList<String>();
                        for(int i = 1 ; i < arr.length ; i++){
                            list.add(arr[i]);
                        }


                        String uuid = Md5Util.getUUID();


                            if(space>=9&&space<=11){
                                totalList.add(list);
                                String sql = "insert into sdlm_sys_断面分层水力参数 (SN_ID,计算目标_ID,河段名称,计算断面号,层号,河段级别,距离下游断面的距离,剖面层数个数,河底高程) values(?,?,?,?,?,?,?,?,?)";
                                ps = connection.prepareStatement(sql);
                                ps.setString(1, uuid);
                                ps.setString(2, "928FE3EF219048C5BFEF749785476164");
                                ps.setString(3, "观音阁水库下游至唐马寨");
                                ps.setInt(4, Integer.valueOf(list.get(0)));
                                ps.setInt(5, 0);
                                ps.setInt(6, Integer.valueOf(list.get(1)));
                                ps.setString(7, String.format("%.4f", Float.parseFloat(list.get(2))));
                                ps.setInt(8, Integer.valueOf(list.get(3)));
                                ps.setString(9, String.format("%.4f", Float.parseFloat(list.get(4))));
                                ps.execute();
                            }
                            if(space==5||space==6){
                                List<String> currentList = (List<String>) totalList.get(totalList.size() - 1);
                                String uuid1 = Md5Util.getUUID();
                                String sql2 = "insert into sdlm_sys_断面分层水力参数 (SN_ID,计算目标_ID,河段名称,计算断面号,层号,层高程,层河宽,层面积,层水力半径) values(?,?,?,?,?,?,?,?,?)";
                                ps = connection.prepareStatement(sql2);
                                ps.setString(1,uuid1);
                                ps.setString(2,"928FE3EF219048C5BFEF749785476164");
                                ps.setString(3, "观音阁水库下游至唐马寨");
                                ps.setInt(4, Integer.valueOf(currentList.get(0)));
                                ps.setInt(5, Integer.valueOf(list.get(0)));
                                ps.setString(6, String.format("%.4f", Float.parseFloat(list.get(1))));
                                ps.setString(7, String.format("%.4f", Float.parseFloat(list.get(2))));
                                ps.setString(8, String.format("%.4f", Float.parseFloat(list.get(3))));
                                ps.setString(9, String.format("%.4f", Float.parseFloat(list.get(4))));
                                ps.execute();
                            }
                        ps.close();
                        ps = null;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

    }

    //导入dxdy.inp
    @Test
    public void importDxdy(){
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);
            String sql = "update yqmx_sys_网格信息表 set DXDY_DX = ? ,DXDY_DY = ? ,DXDY_DEPTH = ? ,DXDY_ELEV = ?, DXDY_ZROUGH = ? where I_CODE = ? and  J_CODE = ?";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\dxdy.inp")));
            String line ;
            int count = 0;
            while ((line = reader.readLine()) != null){
                count++;
                if(count>4){
                    String newLine = line.replaceAll("\\s{1,}", " ");
                    //以空格分隔，生成数组
                    String[] arr = newLine.split(" ");

                    String i = arr[1];
                    String j = arr[2];
                    String dx = arr[3];
                    String dy = arr[4];
                    String depth = arr[5];
                    String elev = arr[6];
                    String zrough = arr[7];

                    ps.setString(1,dx);
                    ps.setString(2,dy);
                    ps.setString(3,depth);
                    ps.setString(4,elev);
                    ps.setString(5,zrough);
                    ps.setString(6,i);
                    ps.setString(7,j);
                    ps.execute();
                }else{
                    continue;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //导入lxly.inp
    @Test
    public void importLxly(){
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);
            String sql = "update yqmx_sys_网格信息表 set LXLY_X = ? ,LXLY_Y = ? ,LXLY_CUE = ? ,LXLY_CVE = ?, LXLY_CUN = ? ,LXLY_CVN = ?, LXLY_WIND = ? where I_CODE = ? and  J_CODE = ?";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\lxly.inp")));
            String line ;
            int count = 0;
            while ((line = reader.readLine()) != null){
                count++;
                if(count>4){
                    String newLine = line.replaceAll("\\s{1,}", " ");
                    //以空格分隔，生成数组
                    String[] arr = newLine.split(" ");

                    String i = arr[1];
                    String j = arr[2];
                    String LXLY_X = arr[3];
                    String LXLY_Y = arr[4];
                    String LXLY_CUE = arr[5];
                    String LXLY_CVE = arr[6];
                    String LXLY_CUN = arr[7];
                    String LXLY_CVN = arr[8];
                    String LXLY_WIND = arr[9];

                    ps.setString(1,LXLY_X);
                    ps.setString(2,LXLY_Y);
                    ps.setString(3,LXLY_CUE);
                    ps.setString(4,LXLY_CVE);
                    ps.setString(5,LXLY_CUN);
                    ps.setString(6,LXLY_CVN);
                    ps.setString(7,LXLY_WIND);
                    ps.setString(8,i);
                    ps.setString(9,j);

                    ps.execute();
                }else{
                    continue;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //导入corners.inp
    @Test
    public void importCorners(){
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);
            String sql = "update yqmx_sys_网格信息表 set CON_BL_X = ? ,CON_BL_Y = ? ,CON_TL_X = ? ,CON_TL_Y = ?, CON_TR_X = ? ,CON_TR_Y = ?, CON_BR_X = ?, CON_BR_Y = ? where I_CODE = ? and  J_CODE = ?";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\corners.inp")));
            String line ;
            int count = 0;
            while ((line = reader.readLine()) != null){
                count++;
                if(count>2){
                    String newLine = line.replaceAll("\\s{1,}", " ");
                    //以空格分隔，生成数组
                    String[] arr = newLine.split(" ");

                    String i = arr[1];
                    String j = arr[2];
                    String x1 = arr[3];
                    String y1 = arr[4];
                    String x2 = arr[5];
                    String y2 = arr[6];
                    String x3 = arr[7];
                    String y3 = arr[8];
                    String x4 = arr[9];
                    String y4 = arr[10];

                    ps.setString(1,x1);
                    ps.setString(2,y1);
                    ps.setString(3,x2);
                    ps.setString(4,y2);
                    ps.setString(5,x3);
                    ps.setString(6,y3);
                    ps.setString(7,x4);
                    ps.setString(8,y4);
                    ps.setString(9,i);
                    ps.setString(10,j);

                    ps.execute();
                }else{
                    continue;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //导入UtmLongitude.OUT
    @Test
    public void importUtmLongitude(){
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);

            String sql = "insert into yqmx_sys_网格信息表 (OBJ_ID,计算目标_ID,L_CODE,I_CODE,J_CODE,UTM_X,UTM_Y,经度,纬度) values(?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\UtmLongitude.OUT")));
            String line ;
            int count = 0;
            while ((line = reader.readLine()) != null){
                count++;
                if(count>2){
                    String uuid = Md5Util.getUUID();
                    String newLine = line.replaceAll("\\s{1,}", " ");
                    //以空格分隔，生成数组
                    String[] arr = newLine.split(" ");
                    ps.setString(1,uuid);
                    ps.setString(2,"4c5172e3e578452e80c4a7a345773eb4");
                    for(int i = 0 ; i < arr.length ; i++){
                        int index = i+3;
                        ps.setString(index,arr[i]);
                    }
                    ps.execute();
                }else{
                    continue;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //导入EE_VEL.PUT文件
    @Test
    public void importEE_VEL(){
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);

            String sql = "insert into yqmx_output_计算结果 (SN_ID,VEL_N,VEL_TIME,VEL_DT,VEL_L_CODE,VEL_X_VALUE,VEL_Y_VALUE,VEL_Z_VELUE,STEP_ID) values(?,?,?,?,?,?,?,?,?)";
            ps = connection.prepareStatement(sql);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_VEL.OUT")));
            String line ;
            int count = 0;
            List<Integer> countList = new ArrayList<Integer>();
            List<String[]> firstData = new ArrayList<String[]>();
//            List<Object> secondData = new ArrayList<Object>();

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
            BufferedReader reader1 = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_VEL.OUT")));
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
                        ps.setString(9,"BF50AC354BG74EA8819659E76E23777E");
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
                    ps.setString(9,"BF50AC354BG74EA8819659E76E23777E");
                    ps.execute();
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }


    //导入EE_WC.OUT文件
    @Test
    public void importEE_WC(){
        Connection connection = null;
        PreparedStatement ps1 = null;
        PreparedStatement ps2 = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);

            String sql1 = "update yqmx_output_计算结果 set WC_TEMPE = ?  where VEL_TIME = ? and VEL_L_CODE = ? and STEP_ID = ?";
            String sql2 = "update yqmx_output_计算结果 set WC_CONC = ?  where VEL_TIME = ? and VEL_L_CODE = ? and STEP_ID = ?";
            ps1 = connection.prepareStatement(sql1);
            ps2 = connection.prepareStatement(sql2);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_WC.OUT")));
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
                    firstData.add(arr[2]);
                    countList.add(count);
                }
            }
            BufferedReader reader1 = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_WC.OUT")));
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
                                ps2.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                                ps2.execute();
                            }else{
                                ps1.setString(1,arr[2]);
                                ps1.setString(2,firstData.get(i));
                                ps1.setString(3,arr[1]);
                                ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                                ps1.execute();
                            }
                        }else{
                            if(rowIndex % 2 == 0){
                                ps1.setString(1,arr[2]);
                                ps1.setString(2,firstData.get(i));
                                ps1.setString(3,arr[1]);
                                ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                                ps1.execute();
                            }else{
                                ps2.setString(1,arr[2]);
                                ps2.setString(2,firstData.get(i));
                                ps2.setString(3,arr[1]);
                                ps2.setString(4,"BF50AC354BG74EA8819659E76E23777E");
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
                            ps2.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                            ps2.execute();
                        }else{
                            ps1.setString(1,arr[2]);
                            ps1.setString(2,firstData.get(countList.size()-1));
                            ps1.setString(3,arr[1]);
                            ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                            ps1.execute();
                        }
                    }else{
                        if(rowIndex % 2 == 0){
                            ps1.setString(1,arr[2]);
                            ps1.setString(2,firstData.get(countList.size()-1));
                            ps1.setString(3,arr[1]);
                            ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                            ps1.execute();
                        }else{
                            ps2.setString(1,arr[2]);
                            ps2.setString(2,firstData.get(countList.size()-1));
                            ps2.setString(3,arr[1]);
                            ps2.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                            ps2.execute();
                        }
                    }
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //导入EE_WS.OUT文件
    @Test
    public void importEE_WS(){
        Connection connection = null;
        PreparedStatement ps1 = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);

            String sql1 = "update yqmx_output_计算结果 set WS_DEPTH = ?  where VEL_TIME = ? and VEL_L_CODE = ? and step_id = ?";
            ps1 = connection.prepareStatement(sql1);
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_WS.OUT")));
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
            BufferedReader reader1 = new BufferedReader(new InputStreamReader(new FileInputStream("D:\\工作\\水环境所预警系统\\于桥水库\\yuqiaoshuiku_exe_input_output\\yuqiaoshuiku_exe_input_output\\#output\\EE_WS.OUT")));
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
                        ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                        ps1.execute();
                    }
                }
                if(rowIndex>countList.get(countList.size()-1)){
                    String newLine = line1.replaceAll("\\s{1,}", " ");
                    String[] arr = newLine.split(" ");
                    ps1.setString(1,arr[2]);
                    ps1.setString(2,firstData.get(countList.size()-1));
                    ps1.setString(3,arr[1]);
                    ps1.setString(4,"BF50AC354BG74EA8819659E76E23777E");
                    ps1.execute();
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                connection.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    @Test
    public void insertData() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            connection = DriverManager.getConnection(url, user, password);
            String sql = "insert into yqmx_biz_运算参数表 (SN_ID,方案_NO) values(?,?)";
            ps = connection.prepareStatement(sql);
            for(int i = 0 ; i < 4525 ; i++){
                String uuid = Md5Util.getUUID();
                ps.setString(1,uuid);
                ps.setString(2,"86110EFC9F1F4CDF88BF175B5E1D3607");
                ps.execute();

            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
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


    @Test
    public void exportData(){
        Connection connection = null;
        PreparedStatement ps1 = null;
        ResultSet rs1 = null;
        PreparedStatement ps2 = null;
        ResultSet rs2 = null;
        List<Map<String,Object>> list = new ArrayList<Map<String, Object>>();
        try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
            String url = "jdbc:oracle:thin:@192.169.11.84:1521:wrEnv";
            String user = "zdwarner";
            String password = "H9000ems";
            try {
                connection = DriverManager.getConnection(url, user, password);
                String sql1 = "select * from sdlm_sys_水质断面对应 t order by t.水质归属河段 asc,t.水质计算断面号 asc";
                String sql2 = "select t2.key_abbr, t2.key_name, t1.value\n" +
                        "  from sdlm_biz_方案条件设定表 t1, sdlm_sys_条件项目定义表 t2\n" +
                        " where t1.step_id = '1111111111111'\n" +
                        "   and t1.水质断面对应_id = ?\n" +
                        "   and t1.key = t2.key_id\n" +
                        "   order by t2.key_name asc";
                ps1 = connection.prepareStatement(sql1);
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

                    ps2 = connection.prepareStatement(sql2);
                    ps2.setString(1,dmId);
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
                    //处理初始浓度和排放浓度的顺序
//                    asc(start,"keyName");
//                    asc(discharge,"keyName");

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
                asc(startKeyList);
                asc(dischargeKeyList);
                BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("E:\\export\\DXQC.txt")));
                StringBuilder stringBuilder = new StringBuilder();
                stringBuilder.append("Segment"+"  ");
                stringBuilder.append("Section" + "  ");
                stringBuilder.append("distance" + "  ");
                for(int k = 0 ; k < startKeyList.size() ; k++){
                    stringBuilder.append(startKeyList.get(k) + "  ");
                }
                stringBuilder.append("flow"+"  ");
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
            } catch (SQLException e) {
                e.printStackTrace();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }try {
            rs1.close();
            rs2.close();
            ps1.close();
            ps2.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public List<Map<String,Object>> asc (List<Map<String,Object>> list, final String key){
        Collections.sort(list, new Comparator<Map<String, Object>>() {
            public int compare(Map<String, Object> o1, Map<String, Object> o2) {
                int map1value = ((String)o1.get(key)).charAt(3);
                int map2value = ((String)o2.get(key)).charAt(3);
                return map1value - map2value;
            }
        });
        return list;
    }

    public List<String> asc (List<String> list){
        Collections.sort(list, new Comparator<String>() {
            public int compare(String o1, String o2) {
                int map1value = o1.charAt(6);
                int map2value = o2.charAt(6);
                return map1value - map2value;
            }
        });
        return list;
    }
    @Test
    public void test(){
//        System.out.println(model.exportDataFile("1111111111111","E:\\DXQC.txt"));
//       System.out.println(model.exportCBOUData("1111111111111","E:\\CBOU.txt"));
//        System.out.println(model.exportCTOTALData("1111111111111", "E:\\CTOTAL.dat"));
        Map<String,String> map1 = new HashMap<String,String>();
        map1.put("mx_internal_uid","E3F5D833DC2D30C035EC55EBE000A374");
        map1.put("name", "氨氮");
        map1.put("num", "23");
        Set<String> keys = map1.keySet();
        for (String key : keys) {
            System.out.println(key);
        }
        System.out.println("-------------------------");
        Iterator key = keys.iterator();
        while (key.hasNext()){
            System.out.println(key.next());
        }

        System.out.print("---ddd----");

        Map<String,String> map2 = new LinkedHashMap<String,String>();
        map2.put("mx_internal_uid","E3F5D833DC2D30C035EC55EBE000A374");
        map2.put("name", "硝酸盐氮");
        map2.put("num", "234");
        Set<String> keys2 = map2.keySet();
        for (String key2 : keys2) {
            System.out.println(key2);
        }
        System.out.println("-------------------------");
        Iterator key2 = keys2.iterator();
        while (key2.hasNext()){
            System.out.println(key2.next());
        }

//        queryService.saveWaterPollutionEvent("d89adf75eddd48bdaf4fa60968dc72b2", "928FE3EF219048C5BFEF749785476164", "测试22", "2015-12-03 15:32:00", "2015-12-04 12:00:00", "太子河", "0", "123.10", "23", "这是一个测试", new Object[]{map1, map2});
//        System.out.print(queryService.saveWaterPollutionEvent("{\n" +
//                "    \"side\":\"1\",\n" +
//                "    \"wrwInfo\":[\n" +
//                "        {\n" +
//                "            \"num\":\"2255\",\n" +
//                "            \"name\":\"3444\",\n" +
//                "            \"mx_internal_uid\":\"EC864CC20B2733D564926978042478CB\"\n" +
//                "        },\n" +
//                "        {\n" +
//                "            \"num\":\"22\",\n" +
//                "            \"name\":\"33\",\n" +
//                "            \"mx_internal_uid\":\"E89767A943E1551CF32969780B82BA64\"\n" +
//                "        }\n" +
//                "    ],\n" +
//                "    \"Latiude\":\"33.33\",\n" +
//                "    \"describe\":\"222222222222\",\n" +
//                "    \"eventName\":\"rr\",\n" +
//                "    \"longtude\":\"123.33\",\n" +
//                "    \"startTime\":\"2016-09-27 10:27:13\",\n" +
//                "    \"endTime\":\"2016-09-27 10:27:13\",\n" +
//                "    \"eventId\":\"d6dfabc76a47439899190a83d437e7d4\",\n" +
//                "    \"waterbodyid\":\"928FE3EF219048C5BFEF749785476164\",\n" +
//                "    \"place\":\"ee\"\n" +
//                "}"));
//            System.out.print(queryService.deleteWaterPollutionEvent("9f987f8a5ee84c318abb5c9344a118f9"));
    }
}
