package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import bitc.sz.wzEnv.dao.ExecuteExeSZPJDao;
import bitc.sz.wzEnv.filter.WebServiceContextFilter;
import bitc.sz.wzEnv.service.ExecuteExeSZPJModel;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.net.URLEncoder;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zsz on 2016/5/12.
 */
@Service
@Transactional
public class ExecuteExeSDLModelImpl implements bitc.sz.wzEnv.service.ExecuteExeSDLModel {
    @Autowired
    private ExecuteExeSZPJModel szpjModel;
    @Override
    public boolean writeCOEDA(File file, String plan_id,ExecuteExeSDLDao dao) throws Exception {
//        String[] blocks = {"一般的", "支流", "流域面积", "闸门", "水库", "流量", "其它"};
        List<Map<Integer, String>> list = dao.getCOEDADate(plan_id);
        Map<Integer, String> map = null;
        BufferedWriter writer = null;
        try {
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file)));
            // 一般的
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(0);
            String result = null;
            for (int i = 0; i < 13; i++) {
                result = map.get(i);
                if (!StringUtils.isEmpty(result)) {
                    writer.write(result);
                    writer.newLine();
                }
            }
            //支流
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(1);
            result = map.get(0);
            // nr行
            if (!StringUtils.isEmpty(result)) {
                writer.write(result);
                writer.newLine();
            }

            result = map.get(1);

            String[] zhiliuDuanmainhao = null;
            if (!StringUtils.isEmpty(result)) {
                zhiliuDuanmainhao = result.split(",");
                for (int i = zhiliuDuanmainhao.length - 1; i >= 0; i--) {
                    writer.write(zhiliuDuanmainhao[i] + " ");
                }
            }
            writer.newLine();
            result = map.get(2);
            String[] zhiliuName = null;
            if (!StringUtils.isEmpty(result)) {
                zhiliuName = result.split(",");
            }
            for (int i = 0; i < zhiliuName.length; i++) {
                writer.write(zhiliuName[i]);
                writer.newLine();
            }
            //流域面积
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(2);
            result = map.get(0);
            if (!StringUtils.isEmpty(result)) {
                writer.write(result.replace(",", " "));
                writer.newLine();
            }
            result = map.get(1);
            if (!StringUtils.isEmpty(result)) {
                String[] zhiliucof = result.split(";");
                for (int i = 0; i < zhiliucof.length; i++) {
                    writer.write(zhiliucof[i].replace(",", " "));
                    writer.newLine();
                }
            }
            //闸门
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(3);
            result = map.get(0);
            //闸门个数行
            if (!StringUtils.isEmpty(result)) {
                writer.write(result);
                writer.newLine();
            }
            //闸门对应断面号行
            result = map.get(1);
            //闸门断面号
            if (!StringUtils.isEmpty(result)) {
                String[] zhamens = result.split(";");
                for (int i = 0; i < zhamens.length; i++) {
                    writer.write(zhamens[i].replace(","," "));
                    writer.newLine();
                }
            }
            //水库
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(4);
            result = map.get(0);
            if (!StringUtils.isEmpty(result)) {
                String[] str = result.split(";");
                for(int i=0;i<str.length;i++){
                    writer.write(str[0].replace(","," "));
                    writer.newLine();
                }
            }
            //流量
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(5);
            for (int i = 0; i < map.size(); i++) {
                result = map.get(i);
                if (!StringUtils.isEmpty(result)) {
                    writer.write(result.replace(",", " "));
                    writer.newLine();
                }
            }
            //其他
            writer.write("*-----*-----*-----*-----*-----*-----*-----*-----*");
            writer.newLine();
            map = list.get(6);
            for (int i = 0; i < 5; i++) {
                result = map.get(i);
                if (!StringUtils.isEmpty(result)) {
                    writer.write(result + " ");
                }
            }
            writer.newLine();
            for (int i = 5; i < 10; i++) {
                result = map.get(i);
                if (!StringUtils.isEmpty(result)) {
                    writer.write(result + " ");
                }
            }
            writer.newLine();
            for (int i = 10; i < 13; i++) {
                result = map.get(i);
                if (!StringUtils.isEmpty(result)) {
                    writer.write(result + " ");
                }
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            writer.close();
        }
    }

    @Override
    public boolean writeGateHYO(File file, String[] names, ExecuteExeSDLDao dao) throws Exception {
        int count = dao.getGateHYOCount();
        String[][] result = dao.getGateHYOData(names);
        BufferedWriter writer = null;
        StringBuilder line = null;
        try {
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file)));
            for (int i = 0; i < names.length; i++) {
                writer.write(names[i] + "\t\t");
            }
            writer.newLine();
            for (int i = 0; i < names.length; i++) {
                writer.write("H上游\tQ下游\t");
            }
            writer.newLine();
            for (int i = 0; i < count; i++) {
                line = new StringBuilder();
                for (int j = 0; j < names.length; j++) {
                    line.append(result[0][count * j + i] + "\t");
                    line.append(result[1][count * j + i] + "\t");
                }
                writer.write(line.toString());
                writer.newLine();
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            writer.close();
        }
    }

    @Override
    public ExtjsResponse waterDMPositionData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        Map<String, Object> m = new HashMap<String, Object>();
        Map<String, Object> m1 = new HashMap<String, Object>();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, obj_id, "73476806f527419686d18gbca8485689", start, end);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            m.put("type", "LineChart");
            m.put("schemeName", map.get("schemeName"));
            m.put("schemeCode", map.get("schemeCode"));
            m.put("sectionName", map.get("sectionName"));
            m.put("sectionCode", map.get("sectionCode"));
            m.put("year", map.get("year"));
            m.put("label", "断面计算水位");
            m.put("color", "0x2C2CD2");
            m.put("xField", "time");
            m.put("yField", "value");
            m.put("xFieldName", "时间");
            m.put("yFieldName", "断面水位");
            m.put("nodeList", list);
            if ("8A46EF3D745D432EA20547AF6C414782".equals(dmId) && "38d5d1f65bc24641a6010759b949473b".equals(obj_id)) {
                List<Map<String, String>> list1 = dao.waterDMJCData(planId, "水位", start, end);
                Map<String, String> map1 = dao.waterDMMetaData(planId, dmId);
                m1.put("type", "plotchart");
                m1.put("schemeName", map1.get("schemeName"));
                m1.put("schemeCode", map1.get("schemeCode"));
                m1.put("sectionName", map1.get("sectionName"));
                m1.put("sectionCode", map1.get("sectionCode"));
                m1.put("year", map1.get("year"));
                m1.put("label", "断面监测水位");
                m1.put("color", "0x8F8FD2");
                m1.put("xField", "time");
                m1.put("yField", "value");
                m1.put("xFieldName", "时间");
                m1.put("yFieldName", "断面水位");
                m1.put("nodeList", list1);
            } else {
                List<Map<String, String>> list1 = dao.waterDMJCData(planId, dmId, "水位", start, end);
                Map<String, String> map1 = dao.waterDMMetaData(planId, dmId);
                m1.put("type", "plotchart");
                m1.put("schemeName", map1.get("schemeName"));
                m1.put("schemeCode", map1.get("schemeCode"));
                m1.put("sectionName", map1.get("sectionName"));
                m1.put("sectionCode", map1.get("sectionCode"));
                m1.put("year", map1.get("year"));
                m1.put("label", "断面监测水位");
                m1.put("color", "0x8F8FD2");
                m1.put("xField", "time");
                m1.put("yField", "value");
                m1.put("xFieldName", "时间");
                m1.put("yFieldName", "断面水位");
                m1.put("nodeList", list1);
            }
            response.addAttribute("targetList", new Map[]{m, m1});
        } catch (Exception e) {
            e.printStackTrace();
        }
        //     System.out.println("waterPositionData:------------------------------------------");
        //     System.out.println("方案ID:" + planId);
        //     System.out.println("断面ID:" + dmId);
        //     System.out.println("开始时间:" + start);
        //     System.out.println("结束时间" + end);
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse waterDMDepthData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, obj_id, "73476806f527419686d18gbca8485689", start, end);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            response.addAttribute("type", "LineChart");
            response.addAttribute("schemeName", map.get("schemeName"));
            response.addAttribute("schemeCode", map.get("schemeCode"));
            response.addAttribute("sectionName", map.get("sectionName"));
            response.addAttribute("sectionCode", map.get("sectionCode"));
            response.addAttribute("year", map.get("year"));
            response.addAttribute("label", "断面水深");
            response.addAttribute("color", "#8F8FD2");
            response.addAttribute("xField", "time");
            response.addAttribute("yField", "value");
            response.addAttribute("xFieldName", "时间");
            response.addAttribute("yFieldName", "水深");
            response.addAttribute("nodeList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        //     System.out.println("waterPositionData:------------------------------------------");
        //     System.out.println("方案ID:" + planId);
        //     System.out.println("断面ID:" + dmId);
        //     System.out.println("开始时间:" + start);
        //     System.out.println("结束时间" + end);
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse waterDMSpeedData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, obj_id, "73476806f527419686d18gbca8485689", start, end);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            response.addAttribute("type", "LineChart");
            response.addAttribute("schemeName", map.get("schemeName"));
            response.addAttribute("schemeCode", map.get("schemeCode"));
            response.addAttribute("sectionName", map.get("sectionName"));
            response.addAttribute("sectionCode", map.get("sectionCode"));
            response.addAttribute("year", map.get("year"));
            response.addAttribute("label", "断面流速");
            response.addAttribute("color", "#8F8FD2");
            response.addAttribute("xField", "time");
            response.addAttribute("yField", "value");
            response.addAttribute("xFieldName", "时间");
            response.addAttribute("yFieldName", "流速（m/s）");
            response.addAttribute("nodeList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        //     System.out.println("waterPositionData:------------------------------------------");
        //     System.out.println("方案ID:" + planId);
        //     System.out.println("断面ID:" + dmId);
        //     System.out.println("开始时间:" + start);
        //     System.out.println("结束时间" + end);
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse waterDMFlowData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        Map<String, Object> m = new HashMap<String, Object>();
        Map<String, Object> m1 = new HashMap<String, Object>();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, obj_id, "73476806f527419686d18gbca8485689", start, end);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            m.put("type", "LineChart");
            m.put("schemeName", map.get("schemeName"));
            m.put("schemeCode", map.get("schemeCode"));
            m.put("sectionName", map.get("sectionName"));
            m.put("sectionCode", map.get("sectionCode"));
            m.put("year", map.get("year"));
            m.put("label", "断面计算流量");
            m.put("color", "0x2C2CD2");
            m.put("xField", "time");
            m.put("yField", "value");
            m.put("xFieldName", "时间");
            m.put("yFieldName", "流量");
            m.put("nodeList", list);
            //渗窝水库
            if ("FE6319G3FAAD4BA2BAE8964A8B0E94C3".equals(dmId) && "e6e886de83254958a024f33e010700aa".equals(obj_id)) {
                List<Map<String, String>> list1 = dao.waterDMJCData(planId, "流量", start, end);
                Map<String, String> map1 = dao.waterDMMetaData(planId, dmId);
                m1.put("type", "plotchart");
                m1.put("schemeName", map1.get("schemeName"));
                m1.put("schemeCode", map1.get("schemeCode"));
                m1.put("sectionName", map1.get("sectionName"));
                m1.put("sectionCode", map1.get("sectionCode"));
                m1.put("year", map1.get("year"));
                m1.put("label", "断面监测流量");
                m1.put("color", "0x8F8FD2");
                m1.put("xField", "time");
                m1.put("yField", "value");
                m1.put("xFieldName", "时间");
                m1.put("yFieldName", "断面水位");
                m1.put("nodeList", list1);
            } else {
                List<Map<String, String>> list1 = dao.waterDMJCData(planId, dmId, "流量", start, end);
                Map<String, String> map1 = dao.waterDMMetaData(planId, dmId);
                m1.put("type", "plotchart");
                m1.put("schemeName", map1.get("schemeName"));
                m1.put("schemeCode", map1.get("schemeCode"));
                m1.put("sectionName", map1.get("sectionName"));
                m1.put("sectionCode", map1.get("sectionCode"));
                m1.put("year", map1.get("year"));
                m1.put("label", "断面监测流量");
                m1.put("color", "0x8F8FD2");
                m1.put("xField", "time");
                m1.put("yField", "value");
                m1.put("xFieldName", "时间");
                m1.put("yFieldName", "断面水位");
                m1.put("nodeList", list1);
            }
            response.addAttribute("targetList", new Map[]{m, m1});
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            response.setAttributes(null);
            e.printStackTrace();
        }
        //     System.out.println("waterPositionData:------------------------------------------");
        //     System.out.println("方案ID:" + planId);
        //     System.out.println("断面ID:" + dmId);
        //     System.out.println("开始时间:" + start);
        //     System.out.println("结束时间" + end);
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse waterHDData(String planId, String obj_id, String obj_id1, String start, String end, ExecuteExeSDLDao dao, String name) {
        String[] colors = {"0x8F8FD2", "0x8F00D2", "0x008FD2", "0x558FD2", "0x8F22D2", "0x8FCC77", "0x8F3366", "0xD28F55", "0x8FAF44", "0x8D233", "0xFF8F22", "0x8F0011"};
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                int year = dao.getPlanYear(planId);
                start = year + "-01-01";
                end = year + "-12-31";
            }
            String currentMonth = null;
            String lastMonth = null;
            List<Map<String, String>> result = null;
            List<Map<String, Object>> rsList = new ArrayList<Map<String, Object>>();

            Map<String, String> rsMap = null;
            Map<String, Object> item = new HashMap<String, Object>();
            Map<String, String> map = dao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDData(planId, "73476806f527419686d18gbca8485689", obj_id, obj_id1, start, end);
            Map<String, String> listMap = null;
            int monthIndex = 0;
            for (int i = 0; i < list.size(); i++) {
                listMap = list.get(i);
                currentMonth = listMap.get("month");
                if (lastMonth == null) {
                    lastMonth = currentMonth;
                    item = new HashMap<String, Object>();
                    item.put("type", "LineChart");
                    item.put("schemeName", map.get("schemeName"));
                    item.put("schemeCode", map.get("schemeCode"));
                    item.put("sectionName", map.get("sectionName"));
                    item.put("sectionCode", map.get("sectionCode"));
                    item.put("year", map.get("year"));
                    item.put("label", currentMonth);
                    item.put("color", colors[monthIndex]);
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                } else if (lastMonth.equals(currentMonth)) {
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                } else {
                    lastMonth = currentMonth;
                    item.put("nodeList", result);
                    rsList.add(item);
                    monthIndex++;
                    item = new HashMap<String, Object>();
                    item.put("type", "LineChart");
                    item.put("schemeName", map.get("schemeName"));
                    item.put("schemeCode", map.get("schemeCode"));
                    item.put("sectionName", map.get("sectionName"));
                    item.put("sectionCode", map.get("sectionCode"));
                    item.put("year", map.get("year"));
                    item.put("label", currentMonth);
                    item.put("color", colors[monthIndex]);
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                }
            }
            if (result != null) {
                item.put("nodeList", result);
            }
            rsList.add(item);
            response.addAttribute("result", rsList);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        //     System.out.println("waterPositionData:------------------------------------------");
//        //     System.out.println("方案ID:" + planId);
//        //     System.out.println("开始时间:" + start);
//        //     System.out.println("结束时间" + end);
//        //     System.out.println(JsonUtil.getJsonString(response));
//        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse waterHDDataByDay(String planId, String obj_id, String obj_id1, String start, String end, ExecuteExeSDLDao dao, String name) {
        String[] colors = {"0x8F8FD2", "0x8F00D2", "0x008FD2", "0x558FD2", "0x8F22D2", "0x8FCC77", "0x8F3366", "0xD28F55", "0x8FAF44", "0x8D233", "0xFF8F22", "0x8F0011"};
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                int year = dao.getPlanYear(planId);
                start = year + "-01-01";
                end = year + "-12-31";
            }
            String currentDay = null;
            String lastDay = null;
            List<Map<String, String>> result = null;
            List<Map<String, Object>> rsList = new ArrayList<Map<String, Object>>();

            Map<String, String> rsMap = null;
            Map<String, Object> item = new HashMap<String, Object>();
            Map<String, String> map = dao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDDataByDay(planId, "73476806f527419686d18gbca8485689", obj_id, obj_id1, start, end);
            Map<String, String> listMap = null;
            int dayIndex = 0;
            for (int i = 0; i < list.size(); i++) {
                listMap = list.get(i);
                currentDay = listMap.get("date");
                if (lastDay == null) {
                    lastDay = currentDay;
                    item = new HashMap<String, Object>();
                    item.put("type", "LineChart");
                    item.put("schemeName", map.get("schemeName"));
                    item.put("schemeCode", map.get("schemeCode"));
                    item.put("sectionName", map.get("sectionName"));
                    item.put("sectionCode", map.get("sectionCode"));
                    item.put("year", map.get("year"));
                    item.put("label", currentDay);
                    item.put("color", colors[dayIndex]);
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                } else if (lastDay.equals(currentDay)) {
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                } else {
                    lastDay = currentDay;
                    item.put("nodeList", result);
                    rsList.add(item);
                    dayIndex++;
                    if(dayIndex == 12){
                        dayIndex = 0;
                    }
                    item = new HashMap<String, Object>();
                    item.put("type", "LineChart");
                    item.put("schemeName", map.get("schemeName"));
                    item.put("schemeCode", map.get("schemeCode"));
                    item.put("sectionName", map.get("sectionName"));
                    item.put("sectionCode", map.get("sectionCode"));
                    item.put("year", map.get("year"));
                    item.put("label", currentDay);
                    item.put("color", colors[dayIndex]);
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    result.add(rsMap);
                }
            }
            if (result != null) {
                item.put("nodeList", result);
            }
            rsList.add(item);
            response.addAttribute("result", rsList);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        //     System.out.println("waterPositionData:------------------------------------------");
//        //     System.out.println("方案ID:" + planId);
//        //     System.out.println("开始时间:" + start);
//        //     System.out.println("结束时间" + end);
//        //     System.out.println(JsonUtil.getJsonString(response));
//        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public boolean writeTaiZiHQ(File file, String id, ExecuteExeSDLDao dao, String planId) throws Exception {
        BufferedWriter writer = null;
        try {
            List<Map<Integer, String>> list = dao.getTaiZiHQDate(planId, id);
            Map<Integer, String> map = null;
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file)));
            map = list.get(0);
            String[] date = map.get(0).split("-");
            String shuiwei = map.get(1);
            String liuliang = map.get(2);
            if (shuiwei.indexOf(".") < 0) {
                shuiwei = shuiwei + ".0";
            }
            if (liuliang.indexOf(".") < 0) {
                liuliang = liuliang + ".0";
            }
            writer.write("5 ");
            writer.write(date[0] + "   ");
            int m = Integer.parseInt(date[1]);
            int d = Integer.parseInt(date[2]);
            if (m < 10) {
                writer.write(" " + m);
            } else {
                writer.write(m + "");
            }
            writer.write("/");
            if (d < 10) {
                writer.write(" " + d);
            } else {
                writer.write(d + "");
            }
            writer.write("            24.0       ");
            writer.write(shuiwei + "     ");
            writer.write(liuliang);
            writer.write("         1         1         1   年,月,日,小时数、水位、流量");
            writer.newLine();
            for (int i = 1; i < list.size() - 1; i++) {
                map = list.get(i);
                date = map.get(0).split("-");
                shuiwei = map.get(1);
                liuliang = map.get(2);
                if (shuiwei.indexOf(".") < 0) {
                    shuiwei = shuiwei + ".0";
                }
                if (liuliang.indexOf(".") < 0) {
                    liuliang = liuliang + ".0";
                }
                writer.write("  " + date[0] + "   ");
                m = Integer.parseInt(date[1]);
                d = Integer.parseInt(date[2]);
                if (m < 10) {
                    writer.write(" " + m);
                } else {
                    writer.write(m + "");
                }
                writer.write("/");
                if (d < 10) {
                    writer.write(" " + d);
                } else {
                    writer.write(d + "");
                }
                writer.write("            24.0       ");
                writer.write(shuiwei + "     ");
                writer.write(liuliang);
                writer.newLine();
            }
            map = list.get(list.size() - 1);
            date = map.get(0).split("-");
            shuiwei = map.get(1);
            liuliang = map.get(2);
            if (shuiwei.indexOf(".") < 0) {
                shuiwei = shuiwei + ".0";
            }
            if (liuliang.indexOf(".") < 0) {
                liuliang = liuliang + ".0";
            }
            writer.write("3 ");
            writer.write(date[0] + "   ");
            m = Integer.parseInt(date[1]);
            d = Integer.parseInt(date[2]);
            if (m < 10) {
                writer.write(" " + m);
            } else {
                writer.write(m + "");
            }
            writer.write("/");
            if (d < 10) {
                writer.write(" " + d);
            } else {
                writer.write(d + "");
            }
            writer.write("            24.0       ");
            writer.write(shuiwei + "     ");
            writer.write(liuliang);
            writer.write("         1         1         1");
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(writer != null)
            writer.close();
        }
    }

    @Override
    public boolean writeTaiziTxt(File file, String fileId, ExecuteExeSDLDao dao, String name, String planId) throws Exception {
        BufferedWriter writer = null;
        try {
            List<Map<Integer, String>> list = dao.getBeisDate(planId, fileId);
            Map<Integer, String> map = null;
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file)));
            writer.write(name);
            writer.write("xxxx Discharge");
            writer.newLine();
            writer.write("Time    89Discharge    72Discharge    90Discharge    96Discharge");
            writer.newLine();
            writer.write("Unit    100001    1800    0    t100001    1800    0    100001    1800    0    100001    1800    0");
            writer.newLine();
            for (int i = 0; i < list.size(); i++) {
                map = list.get(i);
                writer.write("        3.51    1.41    3.92    ");
                writer.write(map.get(1) == null ? "" : map.get(1));
                writer.write("                  ");
                writer.write(map.get(0) == null ? "" : map.get(0));
                writer.newLine();
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            writer.close();
        }
    }


    @Override
    public boolean writeTaiZiSC(File file, ExecuteExeSDLDao dao) throws Exception {
        BufferedWriter writer = null;
        try {
            List<Map<Integer, String>> list = dao.writeTaiZiSC();
            Map<Integer, String> map = null;
            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file)));
            for (int i = 0; i < list.size(); i++) {
                map = list.get(i);
                if ("0".equals(map.get(2))) {
                    writer.write("           ");
                    writer.write(map.get(1));
                    writer.write("           ");
                    writer.write(map.get(3));
                    writer.write("   ");
                    writer.write(map.get(4));
                    writer.write("              ");
                    writer.write(map.get(5));
                    writer.write("   ");
                    writer.write(map.get(6));
                    writer.newLine();
                } else {
                    writer.write("      ");
                    writer.write(map.get(2));
                    writer.write("        ");
                    writer.write(map.get(7));
                    writer.write("         ");
                    writer.write(map.get(8));
                    writer.write("         ");
                    writer.write(map.get(9));
                    writer.write("         ");
                    writer.write(map.get(10));
                    writer.newLine();
                }
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            writer.close();
        }
    }

    @Override
    public boolean writeTaiZiHModelInFile(String plan_Id, String path, ExecuteExeSDLDao dao, String[] zhiliuNames, String[] zhamenNames) throws Exception {
        Map<String, String> map = dao.getExecFile(plan_Id);
        for (String key : map.keySet()) {
            if ("TaiZiHQ.dat".equals(key)) {
                if(!writeTaiZiHQ(new File(path + "/" + key), map.get(key), dao, plan_Id)){
                    return false;
                };
            }else {
                for (int i = 0; i < zhiliuNames.length; i++) {
                    String txtName = zhiliuNames[i];
                    if (txtName.equals(key)) {
                        if(!writeTaiziTxt(new File(path + "/" + key), map.get(key), dao, "xxxx", plan_Id)){
                            return false;
                        };
                    }
                }
            }
        }
        if(!writeCOEDA(new File(path + "/COEDA.DAT"),plan_Id,dao)){
            return false;
        };
        if(!writeGateHYO(new File(path + "/GateHYO.txt"), zhamenNames, dao)){
            return false;
        };
        if(!writeTaiZiSC(new File(path + "/TaiZiSC.bth"), dao)){
            return false;
        };
        return true;
    }

    @Override
    public ExtjsResponse getPlanInfoList(String keyword, String water_bady_id, String userid, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.getPlanInfoList(keyword, water_bady_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        //     System.out.println("getPlanInfoList:------------------------------------------");
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("keyword:" + keyword);
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse getDmInfoList(String planId, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.getDmInfoList(planId);
            response.addAttribute("riverCode", "");
            response.addAttribute("riverName", "");
            response.addAttribute("attributes", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        //     System.out.println("getDmInfoList:------------------------------------------");
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("planId:" + planId);
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public ExtjsResponse getTerrainProfileData(String planId, String dmId, String start, String end, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse(true);
        Map<String, Object> m = new HashMap<String, Object>();
        Map<String, Object> m1 = new HashMap<String, Object>();
        try {
            List<Map<String, String>> list = dao.getDMElevation(dmId);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            m1.put("type", "AreaChart");
            m1.put("schemeName", map.get("schemeName"));
            m1.put("schemeCode", map.get("schemeCode"));
            m1.put("sectionName", map.get("sectionName"));
            m1.put("sectionCode", map.get("sectionCode"));
            m1.put("year", map.get("year"));
            m1.put("label", "高程");
            m1.put("color", "0xF5AC39");
            m1.put("xField", "distance");
            m1.put("yField", "altitude");
            m1.put("xFieldName", "距离");
            m1.put("yFieldName", "高程");
            m1.put("nodeList", list);

            List<Map<String, String>> list1 = dao.getDMWaterPosition(planId, dmId, start, end);
            m.put("type", "AreaChart");
            m.put("schemeName", map.get("schemeName"));
            m.put("schemeCode", map.get("schemeCode"));
            m.put("sectionName", map.get("sectionName"));
            m.put("sectionCode", map.get("sectionCode"));
            m.put("year", map.get("year"));
            m.put("label", "水位");
            m.put("color", "0x55D8FE");
            m.put("xField", "date");
            m.put("yField", "altitude");
            m.put("xFieldName", "距离");
            m.put("yFieldName", "高程");
            m.put("nodeList", list1);
            response.addAttribute("targetList", new Map[]{m1, m});
        } catch (Exception e) {
            e.printStackTrace();
        }
        return response;
    }

    @Override
    public void exportDMWaterPositionExcel(String planId, String dmId, ExecuteExeSDLDao dao) throws IOException, SQLException {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            //计算水位
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, "38d5d1f65bc24641a6010759b949473b", "73476806f527419686d18gbca8485689", null, null);
            List<Map<String, String>> list1 = null;
            //监测水位
            if ("82E4D7AB22C04AC09D6A6D14F7948336".equals(dmId)) {
                list1 = dao.waterDMJCData(planId, "水位", null, null);
            } else {
                list1 = dao.waterDMJCData(planId, dmId, "水位", null, null);
            }
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);

            HSSFWorkbook workbook = new HSSFWorkbook();

            HSSFSheet sheet = workbook.createSheet("计算水位");
            HSSFRow head = sheet.createRow(0);
            //第一行标题
            HSSFCell cell = head.createCell(0);
            HSSFCell cell1 = head.createCell(1);
            cell.setCellValue("时间");
            cell1.setCellValue("水位值(m)");
            //从第二行开始数据
            HSSFRow row = null;
            for (int i = 0; i < list.size(); i++) {
                row = sheet.createRow(i + 1);
                cell = row.createCell(0);
                cell.setCellValue(list.get(i).get("time"));
                cell1 = row.createCell(1);
                cell1.setCellValue(list.get(i).get("value"));
            }

            HSSFSheet sheet1 = workbook.createSheet("监测水位");
            HSSFRow head1 = sheet1.createRow(0);
            //第一行标题
            HSSFCell cell2 = head1.createCell(0);
            HSSFCell cell3 = head1.createCell(1);
            cell2.setCellValue("时间");
            cell3.setCellValue("水位值(m)");
            //从第二行开始数据
            HSSFRow row1 = null;
            for (int i = 0; i < list1.size(); i++) {
                row1 = sheet1.createRow(i + 1);
                cell2 = row1.createCell(0);
                cell2.setCellValue(list.get(i).get("time"));
                cell3 = row1.createCell(1);
                cell3.setCellValue(list1.get(i).get("value"));
            }
            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_水位结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void exportDMWaterFlowExcel(String planId, String dmId, ExecuteExeSDLDao dao) throws IOException, SQLException {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            //计算水位
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, "e6e886de83254958a024f33e010700aa", "73476806f527419686d18gbca8485689", null, null);
            List<Map<String, String>> list1 = null;
            //监测水位
            if ("FE6319G3FAAD4BA2BAE8964A8B0E94C3".equals(dmId)) {
                list1 = dao.waterDMJCData(planId, "流量", null, null);
            } else {
                list1 = dao.waterDMJCData(planId, dmId, "流量", null, null);
            }
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);

            HSSFWorkbook workbook = new HSSFWorkbook();

            HSSFSheet sheet = workbook.createSheet("计算流量");
            HSSFRow head = sheet.createRow(0);
            //第一行标题
            HSSFCell cell = head.createCell(0);
            HSSFCell cell1 = head.createCell(1);
            cell.setCellValue("时间");
            cell1.setCellValue("流量值(m3/s)");
            //从第二行开始数据
            HSSFRow row = null;
            for (int i = 0; i < list.size(); i++) {
                row = sheet.createRow(i + 1);
                cell = row.createCell(0);
                cell.setCellValue(list.get(i).get("time"));
                cell1 = row.createCell(1);
                cell1.setCellValue(list.get(i).get("value"));
            }

            HSSFSheet sheet1 = workbook.createSheet("监测流量");
            HSSFRow head1 = sheet1.createRow(0);
            //第一行标题
            HSSFCell cell2 = head1.createCell(0);
            HSSFCell cell3 = head1.createCell(1);
            cell2.setCellValue("时间");
            cell3.setCellValue("流量值(m3/s)");
            //从第二行开始数据
            HSSFRow row1 = null;
            for (int i = 0; i < list1.size(); i++) {
                row1 = sheet1.createRow(i + 1);
                cell2 = row1.createCell(0);
                cell2.setCellValue(list.get(i).get("time"));
                cell3 = row1.createCell(1);
                cell3.setCellValue(list1.get(i).get("value"));
            }
            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_流量结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void exportDMWaterDepthExcel(String planId, String dmId, ExecuteExeSDLDao dao) throws IOException, SQLException {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            //计算水位
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, "ae0f48e69e0d4dc79d4a995303b4d9c9", "73476806f527419686d18gbca8485689", null, null);
            //监测水位
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);

            HSSFWorkbook workbook = new HSSFWorkbook();

            HSSFSheet sheet = workbook.createSheet("水深");
            HSSFRow head = sheet.createRow(0);
            //第一行标题
            HSSFCell cell = head.createCell(0);
            HSSFCell cell1 = head.createCell(1);
            cell.setCellValue("时间");
            cell1.setCellValue("水深(m)");
            //从第二行开始数据
            HSSFRow row = null;
            for (int i = 0; i < list.size(); i++) {
                row = sheet.createRow(i + 1);
                cell = row.createCell(0);
                cell.setCellValue(list.get(i).get("time"));
                cell1 = row.createCell(1);
                cell1.setCellValue(list.get(i).get("value"));
            }
            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_水深结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void exportDMWaterSpeedExcel(String planId, String dmId, ExecuteExeSDLDao dao) throws IOException, SQLException {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            //计算水位
            List<Map<String, String>> list = dao.waterDMData(planId, dmId, "6641d96650694ce2a704dd1b3f71052d", "73476806f527419686d18gbca8485689", null, null);
            //监测水位
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);

            HSSFWorkbook workbook = new HSSFWorkbook();

            HSSFSheet sheet = workbook.createSheet("流速");
            HSSFRow head = sheet.createRow(0);
            //第一行标题
            HSSFCell cell = head.createCell(0);
            HSSFCell cell1 = head.createCell(1);
            cell.setCellValue("时间");
            cell1.setCellValue("流速(m/s)");
            //从第二行开始数据
            HSSFRow row = null;
            for (int i = 0; i < list.size(); i++) {
                row = sheet.createRow(i + 1);
                cell = row.createCell(0);
                cell.setCellValue(list.get(i).get("time"));
                cell1 = row.createCell(1);
                cell1.setCellValue(list.get(i).get("value"));
            }
            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_流速结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public ExtjsResponse getCompareblePlanList(String planId, ExecuteExeSDLDao dao) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = dao.getCompareblePlanList(planId);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        //     System.out.println("getCompareblePlanList:------------------------------------------");
        //     System.out.println(JsonUtil.getJsonString(response));
        //     System.out.println("planId:" + planId);
        //     System.out.println("----------------------------------------------------------");
        return response;
    }

    @Override
    public void exportHDWaterDataExcel(String planId,String modelId, String jisuanId, ExecuteExeSDLDao dao) throws Exception {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            int year = dao.getPlanYear(planId);
            String start = year + "-01-01";
            String end = year + "-12-31";
            Map<String, String> map = dao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDData(planId, modelId, "8476eb918f9743a0b3053f403bb8bb22", jisuanId, start, end);
            HSSFWorkbook workbook = new HSSFWorkbook();
            String name = dao.getJisuanName(jisuanId);
            String danwei = "";
            if("断面水位".equals(name)){
                danwei="(m)";
            }else if("断面水深".equals(name)){
                danwei="(m)";
            }else if("断面流速".equals(name)){
                danwei="(m/s)";
            }else if("流量".equals(name)){
                danwei="(m3/s)";
            }
            HSSFSheet sheet = workbook.createSheet("河道" + name);
            HSSFRow head = sheet.createRow(0);
            //第一行标题
            HSSFCell cell = null;
            HSSFCell cell1 = null;
            HSSFCell cell2 = null;
            //从第二行开始数据
            HSSFRow row = null;
            String currentMonth = null;
            int col = 0;
            int r = 1;
            for (int i = 0; i < list.size(); i++) {
                if (currentMonth == null) {
                    col = 0;
                    cell = head.createCell(col);
                    cell1 = head.createCell(col + 1);
                    cell2 = head.createCell(col + 2);
                    cell.setCellValue("时间");
                    cell1.setCellValue("距离(m)");
                    cell2.setCellValue(name+danwei);
                    currentMonth = list.get(i).get("month");
                    r = 1;
                } else if (!currentMonth.equals(list.get(i).get("month"))) {
                    col = col + 4;
                    cell = head.createCell(col);
                    cell1 = head.createCell(col + 1);
                    cell2 = head.createCell(col + 2);
                    cell.setCellValue("时间");
                    cell1.setCellValue("距离(m)");
                    cell2.setCellValue(name+danwei);
                    currentMonth = list.get(i).get("month");
                    r = 1;
                }
                row = sheet.getRow(r);
                if (row == null) {
                    row = sheet.createRow(r);
                }
                cell = row.createCell(col);
                cell.setCellValue(currentMonth);
                cell1 = row.createCell(col + 1);
                cell1.setCellValue(list.get(i).get("distance"));
                cell2 = row.createCell(col + 2);
                cell2.setCellValue(list.get(i).get("value"));
                row = null;
                r++;
            }

            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_" + name + "结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void exportHDDensitySourceExcel(String planId, ExecuteExeSDLDao exeDao, ExecuteExeSZPJDao dao) throws Exception {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            int year = exeDao.getPlanYear(planId);
            String start = year + "-01-01";
            String end = year + "-12-31";
            Map<String, String> map = exeDao.waterHDMetaData(planId);
            List<Map<String,String>> wuranwus = dao.getWuRanWuList(planId);
            HSSFWorkbook workbook = new HSSFWorkbook();

            String value = szpjModel.getModelType(planId);
            List<Map<String, String>> wrwData = null;
            for(int s=0;s<wuranwus.size();s++){
                if("1".equals(value)){
                    wrwData = dao.waterHDData(planId, "04b71ddcb30a454ga9ede57198834243", "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(s).get("id"), dao.getJianCeItemId(wuranwus.get(s).get("name")), start, end);
                    HSSFSheet sheet = workbook.createSheet("河道"+wuranwus.get(s).get("name")+"污染物");
                    HSSFRow head = sheet.createRow(0);
                    //第一行标题
                    HSSFCell cell = null;
                    HSSFCell cell1 = null;
                    HSSFCell cell2 = null;
                    //从第二行开始数据
                    HSSFRow row = null;
                    String currentMonth = null;
                    int col = 0;
                    int r = 1;
                    for (int i = 0; i < wrwData.size(); i++) {
                        if (currentMonth == null) {
                            col = 0;
                            cell = head.createCell(col);
                            cell1 = head.createCell(col + 1);
                            cell2 = head.createCell(col + 2);
                            cell.setCellValue("时间");
                            cell1.setCellValue("距离(m)");
                            cell2.setCellValue(wuranwus.get(s).get("name")+"(mg/L)");
                            currentMonth = wrwData.get(i).get("month");
                            r = 1;
                        } else if (!currentMonth.equals(wrwData.get(i).get("month"))) {
                            col = col + 4;
                            cell = head.createCell(col);
                            cell1 = head.createCell(col + 1);
                            cell2 = head.createCell(col + 2);
                            cell.setCellValue("时间");
                            cell1.setCellValue("距离(m)");
                            cell2.setCellValue(wuranwus.get(s).get("name")+"(mg/L)");
                            currentMonth = wrwData.get(i).get("month");
                            r = 1;
                        }
                        row = sheet.getRow(r);
                        if (row == null) {
                            row = sheet.createRow(r);
                        }
                        cell = row.createCell(col);
                        cell.setCellValue(currentMonth);
                        cell1 = row.createCell(col + 1);
                        cell1.setCellValue(wrwData.get(i).get("distance"));
                        cell2 = row.createCell(col + 2);
                        cell2.setCellValue(wrwData.get(i).get("value"));
                        row = null;
                        r++;
                    }
                }else if("2".equals(value)){
                    String stepId = dao.getStepByPlan(planId);
                     start = dao.getSzpjData(stepId, "4D948A512FE1464283F8BB8099D1603E");
                    String zongbuchang =  dao.getSzpjData(stepId,"9A4466629D6449CBAB736EC6DAD01FFD");
                    Calendar calendar = Calendar.getInstance();
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
                    calendar.setTime(sdf.parse(start));
                    calendar.add(Calendar.DATE, Integer.valueOf(zongbuchang) - 1);
                    end = sdf.format(calendar.getTime());
                    wrwData = dao.waterHDSsSource(stepId, "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(s).get("id"),  dao.getJianCeItemId(wuranwus.get(s).get("name")), start, end);
                    HSSFSheet sheet = workbook.createSheet("河道"+wuranwus.get(s).get("name")+"污染物");
                    HSSFRow head = sheet.createRow(0);
                    //第一行标题
                    HSSFCell cell = null;
                    HSSFCell cell1 = null;
                    HSSFCell cell2 = null;
                    //从第二行开始数据
                    HSSFRow row = null;
                    String currentMonth = null;
                    int col = 0;
                    int r = 1;
                    for (int i = 0; i < wrwData.size(); i++) {
                        if (currentMonth == null) {
                            col = 0;
                            cell = head.createCell(col);
                            cell1 = head.createCell(col + 1);
                            cell2 = head.createCell(col + 2);
                            cell.setCellValue("时间");
                            cell1.setCellValue("距离(m)");
                            cell2.setCellValue(wuranwus.get(s).get("name")+"(mg/L)");
                            currentMonth = wrwData.get(i).get("month");
                            r = 1;
                        }else{
                            currentMonth = wrwData.get(i).get("month");
                        } /*else if (!currentMonth.equals(wrwData.get(i).get("month"))) {
                            col = col + 4;
                            cell = head.createCell(col);
                            cell1 = head.createCell(col + 1);
                            cell2 = head.createCell(col + 2);
                            cell.setCellValue("时间");
                            cell1.setCellValue("距离(m)");
                            cell2.setCellValue(wuranwus.get(s).get("name")+"(mg/L)");
                            currentMonth = wrwData.get(i).get("month");
                            r = 1;
                        }*/
                        row = sheet.getRow(r);
                        if (row == null) {
                            row = sheet.createRow(r);
                        }
                        cell = row.createCell(col);
                        cell.setCellValue(currentMonth);
                        cell1 = row.createCell(col + 1);
                        cell1.setCellValue(wrwData.get(i).get("distance"));
                        cell2 = row.createCell(col + 2);
                        cell2.setCellValue(wrwData.get(i).get("value"));
                        row = null;
                        r++;
                    }
                }
            }

            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_污染物结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void exportDMDensitySourceExcel(String planId, String juliId, String dmId, ExecuteExeSDLDao dao, ExecuteExeSZPJDao szpjDao) throws Exception {
        HttpServletResponse response = WebServiceContextFilter.response.get();
        try {
            int year = dao.getPlanYear(planId);
            String start = year + "-01-01";
            String end = year + "-12-31";

            List<Map<String,String>> wuranwus = szpjDao.getWuRanWuList(planId);
            Map<String, String> map = dao.waterDMMetaData(planId, dmId);
            HSSFWorkbook workbook = new HSSFWorkbook();
            for(int s=0;s<wuranwus.size();s++){
                List<Map<String,String>> wrwJs = dao.waterDMData(planId,dmId,wuranwus.get(s).get("id"),"04b71ddcb30a454ga9ede57198834243",start, end);
                List<Map<String,String>> wrwJc =  szpjDao.getDMJiceValue(dmId, szpjDao.getJianCeItemId(wuranwus.get(s).get("name")), start, end);

                HSSFSheet sheet = workbook.createSheet(wuranwus.get(s).get("name")+"计算值");
                HSSFRow head = sheet.createRow(0);
                //第一行标题
                HSSFCell cell = head.createCell(0);
                HSSFCell cell1 = head.createCell(1);
                cell.setCellValue("时间");
                cell1.setCellValue("浓度值(mg/L)");
                //从第二行开始数据
                HSSFRow row = null;
                for (int i = 0; i < wrwJs.size(); i++) {
                    row = sheet.createRow(i + 1);
                    cell = row.createCell(0);
                    cell.setCellValue(wrwJs.get(i).get("time"));
                    cell1 = row.createCell(1);
                    cell1.setCellValue(wrwJs.get(i).get("value"));
                }

                HSSFSheet sheet1 = workbook.createSheet(wuranwus.get(s).get("name")+"检测值");
                HSSFRow head1 = sheet1.createRow(0);
                //第一行标题
                HSSFCell cell2 = head1.createCell(0);
                HSSFCell cell3 = head1.createCell(1);
                cell2.setCellValue("时间");
                cell3.setCellValue("浓度值(mg/L)");
                //从第二行开始数据
                HSSFRow row1 = null;
                for (int i = 0; i < wrwJc.size(); i++) {
                    row1 = sheet1.createRow(i + 1);
                    cell2 = row1.createCell(0);
                    cell2.setCellValue(wrwJc.get(i).get("time"));
                    cell3 = row1.createCell(1);
                    cell3.setCellValue(wrwJc.get(i).get("value"));
                }
            }

            String fileName = map.get("schemeName") + "_" + map.get("sectionName") + "_污染物浓度结果.xls";
            response.reset();
            response.setHeader("Cache-Control", "no-cache");
            response.setHeader("Pragma", "no-cache");
            response.setDateHeader("Expires", -1);
            response.setContentType("application/vnd.ms-excel");
            response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode(fileName, "UTF-8"));
            OutputStream out = response.getOutputStream();
            workbook.write(out);
            out.close();
            workbook.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public String getStepByPlanId(String plan_id,ExecuteExeSDLDao dao) throws SQLException {
       return dao.getStepByPlanId(plan_id);
    }
}
