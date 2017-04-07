package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import bitc.sz.wzEnv.dao.ExecuteExeSZPJDao;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.io.*;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zsz on 2016/6/22.
 */
@Service
@Transactional
public class ExecuteExeSZPJModelImpl implements bitc.sz.wzEnv.service.ExecuteExeSZPJModel {

    @Autowired
    private ExecuteExeSZPJDao dao;
    @Autowired
    private ExecuteExeSDLDao sdlDao;

    @Override
    public boolean readWQPLT2Database(File WQfile, String plan_id) throws Exception {
        String stepId = dao.getStepByPlan(plan_id);
        int year = dao.getYearByPlan(plan_id);
        BufferedReader reader0 = new BufferedReader(new InputStreamReader(new FileInputStream(WQfile)));
        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(WQfile)));
        String line;
        int dateIndex = 0;
        //一天的数据
        List<String[]> strList = null;
        //一行的数据
        String[] strData = null;
        Calendar start = Calendar.getInstance();
        try {
            int countNum = 0;
            //计算出一天多少数据集
            while ((line = reader0.readLine()) != null) {
                if (line.indexOf("ZONE") >= 0) {
                    countNum = 0;
                } else if (countNum != 0 && line.indexOf("TITLE=") >= 0) {
                    break;
                } else {
                    countNum++;
                }
            }

            while ((line = reader.readLine()) != null) {
                if (line.indexOf("TITLE=") >= 0) {
                    dateIndex = Integer.parseInt(line.substring(24, 27).trim());
                    start.set(year, 0, 1);
                    start.add(Calendar.DATE, dateIndex - 1);
                    strList = new ArrayList<String[]>();
                } else if (line.indexOf("VARIABLES") >= 0) {
                } else if (line.indexOf("ZONE") >= 0) {
                } else {
                    strData = line.trim().split("\\s+");
//                    //断面号
//                    strData[0] = line.substring(0, 5).trim();
//                    //距起点距离
//                    strData[1] = line.substring(6, 20).trim();
//                    //NH3-N 浓度
//                    strData[2] = line.substring(21, 30).trim();
//                    // COD 浓度
//                    strData[3] = line.substring(31, 40).trim();

                    strList.add(strData);
                    //一天的数据读取完成，开始保存
                    if (strList.size() == countNum) {
                        dao.readWQPLT2Database(strList, start.getTimeInMillis(), countNum, stepId);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            reader0.close();
            reader.close();
        }
        return false;
    }

    @Override
    public ExtjsResponse waterDMDensitySource(String planId, String dmId, String start, String end) throws Exception {
        ExtjsResponse response = new ExtjsResponse(true);
        try {
            List<Map<String, String>> wuranwus = getWuRanWuList(planId);
            Map[] resultMap = new Map[wuranwus.size()];
            int year = sdlDao.getPlanYear(planId);
            Map<String, String> map = sdlDao.waterDMMetaData(planId, dmId);
            Map<String, Object> map1 = null;
            Map<String, Object> map2 = null;
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                start = year + "-01-01";
                end = year + "-12-31";
            }
            for (int i = 0; i < wuranwus.size(); i++) {
                map1 = new HashMap();
                map2 = new HashMap();
                List<Map<String, String>> wrwJsData = sdlDao.waterDMData(planId, dmId, wuranwus.get(i).get("id"), "04b71ddcb30a454ga9ede57198834243", start, end);
                List<Map<String, String>> wrwJcData = dao.getDMJiceValue(dmId, getJianCeItemId(wuranwus.get(i).get("name")), start, end);

                map1.put("type", "LineChart");
                map1.put("schemeName", map.get("schemeName"));
                map1.put("schemeCode", map.get("schemeCode"));
                map1.put("sectionName", map.get("sectionName"));
                map1.put("sectionCode", map.get("sectionCode"));
                map1.put("year", map.get("year"));
                map1.put("label", wuranwus.get(i).get("name") + "计算浓度");
                map1.put("color", "0x2C2CD2");
                map1.put("xField", "time");
                map1.put("yField", "value");
                map1.put("xFieldName", "时间");
                map1.put("yFieldName", "浓度(mg/l)");
                map1.put("nodeList", wrwJsData);

                map2.put("type", "plotchart");
                map2.put("schemeName", map.get("schemeName"));
                map2.put("schemeCode", map.get("schemeCode"));
                map2.put("sectionName", map.get("sectionName"));
                map2.put("sectionCode", map.get("sectionCode"));
                map2.put("year", map.get("year"));
                map2.put("label", wuranwus.get(i).get("name") + "监测浓度");
                map2.put("color", "0x8F8FD2");
                map2.put("xField", "time");
                map2.put("yField", "value");
                map2.put("xFieldName", "时间");
                map2.put("yFieldName", "浓度(mg/l)");
                map2.put("nodeList", wrwJcData);

                Map<String, Object> result1 = new HashMap<>();
                result1.put("pollutionType", wuranwus.get(i).get("name"));
                result1.put("nodeList", new Map[]{map1, map2});
                resultMap[i] = result1;
            }
            response.addAttribute("targetList", resultMap);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            response.setAttributes(null);
            e.printStackTrace();
            throw e;
        }
        return response;
    }

    @Override
    public List<Map<String, Object>> waterHDNH3NSource(String planId, String juliId, String jisuanId, String item_id, String start, String end, String name) throws Exception {
        String[] colors = {
                "0x8F8FD2", "0x8F00D2", "0x008FD2", "0x558FD2", "0x8F22D2", "0x8FCC77",
                "0x8F3366", "0xD28F55", "0x8FAF44", "0x8D233", "0xFF8F22", "0x8F0011"};
        List<Map<String, Object>> rsList = new ArrayList<Map<String, Object>>();
        try {
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                int year = sdlDao.getPlanYear(planId);
                start = year + "-01-01";
                end = year + "-12-31";
            }
            String currentMonth = null;
            String lastMonth = null;
            List<Map<String, String>> result = null;

            Map<String, String> rsMap = null;
            Map<String, Object> item = new HashMap<String, Object>();
            Map<String, String> map = sdlDao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDData(planId, "04b71ddcb30a454ga9ede57198834243", juliId, jisuanId, item_id, start, end);
//                String stepId = dao.getStepByPlan(planId);
            Map<String, String> listMap = null;
            int monthIndex = 0;
            double maxValue = Double.MIN_VALUE;
            String maxDmName = null;
            String maxOjbId = null;
            Map<String, Object> distance = new HashMap<String, Object>();
            Map<String, String> item1 = new HashMap<String, String>();
            item1.put("name", "本溪市");
            item1.put("begin", "60.33");
            item1.put("end", "71.83");
            item1.put("color", "0xF0F0F0");
            distance.put("x_1", item1);
            item1 = new HashMap<String, String>();
            item1.put("name", "辽阳市");
            item1.put("begin", "141.1");
            item1.put("end", "153.7");
            item1.put("color", "0xF0F0F0");
            distance.put("x_2", item1);
            item.put("x-distance", distance);
            item.put("x_total", "213.45");
            rsList.add(item);
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
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    result.add(rsMap);
                } else if (lastMonth.equals(currentMonth)) {
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    if (Double.parseDouble(listMap.get("value")) > maxValue) {
                        maxValue = Double.parseDouble(listMap.get("value"));
                        maxDmName = listMap.get("dmName");
                        maxOjbId = listMap.get("super_obj_id");
                    }

                    result.add(rsMap);
                } else {
                    lastMonth = currentMonth;
                    item.put("nodeList", result);
                    item.put("对应浓度值", maxValue);
                    item.put("最高浓度断面名称", maxDmName);
                    item.put("super_obj_id", maxOjbId);
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
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    result.add(rsMap);
                }
            }
            if (result != null) {
                item.put("对应浓度值", maxValue);
                item.put("最高浓度断面名称", maxDmName);
                item.put("super_obj_id", maxOjbId);
                item.put("nodeList", result);
            }
            rsList.add(item);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
        return rsList;
    }

    @Override
    public List<Map<String, Object>> waterHDNH3NSourceByDay(String planId, String juliId, String jisuanId, String item_id, String start, String end, String name) throws Exception {
        String[] colors = {
                "0x8F8FD2", "0x8F00D2", "0x008FD2", "0x558FD2", "0x8F22D2", "0x8FCC77",
                "0x8F3366", "0xD28F55", "0x8FAF44", "0x8D233", "0xFF8F22", "0x8F0011"};
        List<Map<String, Object>> rsList = new ArrayList<Map<String, Object>>();
        try {
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                int year = sdlDao.getPlanYear(planId);
                start = year + "-01-01";
                end = year + "-12-31";
            }
            String currentDay = null;
            String lastDay = null;
            List<Map<String, String>> result = null;

            Map<String, String> rsMap = null;
            Map<String, Object> item = new HashMap<String, Object>();
            Map<String, String> map = sdlDao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDDataByDay(planId, "04b71ddcb30a454ga9ede57198834243", juliId, jisuanId, item_id, start, end);
//            String stepId = dao.getStepByPlan(planId);
            Map<String, String> listMap = null;
            int dayIndex = 0;
            double maxValue = Double.MIN_VALUE;
            String maxDmName = null;
            String maxOjbId = null;
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
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    result.add(rsMap);
                } else if (lastDay.equals(currentDay)) {
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    if (Double.parseDouble(listMap.get("value")) > maxValue) {
                        maxValue = Double.parseDouble(listMap.get("value"));
                        maxDmName = listMap.get("dmName");
                        maxOjbId = listMap.get("super_obj_id");
                    }

                    result.add(rsMap);
                } else {
                    lastDay = currentDay;
                    item.put("nodeList", result);
                    item.put("对应浓度值", maxValue);
                    item.put("最高浓度断面名称", maxDmName);
                    item.put("super_obj_id", maxOjbId);
                    rsList.add(item);
                    dayIndex++;
                    if (dayIndex >= 12) {
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
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    result.add(rsMap);
                }
            }
            if (result != null) {
                item.put("对应浓度值", maxValue);
                item.put("最高浓度断面名称", maxDmName);
                item.put("super_obj_id", maxOjbId);
                item.put("nodeList", result);
            }
            rsList.add(item);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
        return rsList;
    }

    @Override
    public boolean exportDataFile(String step_id, String filePath) throws SQLException {
        try {
            return dao.getExportData(step_id, filePath);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public boolean exportCBOUData(String step_id, String filePath) throws SQLException {
        try {
            return dao.exportCBOUData(step_id, filePath);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public boolean exportCgCTOTALData(String step_id, String filePath) throws SQLException {
        try {
            return dao.exportCTOTALData(step_id, filePath);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public boolean writeInputFile(String plan_id, String file, boolean flag) throws Exception {
        String step_id = dao.getStepByPlan(plan_id);
        try {
            if (flag) {
                if (!exportJH7T(step_id, file)) {
                    return false;
                }
                if (!exportSsCTOTALData(step_id, file)) {
                    return false;
                }
            } else {
                if (!exportCBOUData(step_id, file)) {
                    return false;
                }
                if (!exportCgCTOTALData(step_id, file)) {
                    return false;
                }
                if (!exportDataFile(step_id, file)) {
                    return false;
                }

            }
            if (!exportJHXSData(step_id, file, flag)) {
                return false;
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    private boolean exportSsCTOTALData(String step_id, String file) throws SQLException {
        try {
            return dao.exportSsCTOTALData(step_id, file);
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public String getModelType(String plan_id) throws SQLException {
        return dao.getSzpjData(dao.getStepByPlan(plan_id), "0957F099F6814F6397AE740B3A5667FC");
    }

    @Override
    public boolean exportJH7T(String step_id, String file) throws Exception {
        //投放日期
        String riqi = dao.getSzpjData(step_id, "4D948A512FE1464283F8BB8099D1603E");
        //投放量
        String value = dao.getSzpjData(step_id, "6F147C8F4KE2434D881B952108763E6B");
        //污染物集合
        String data = dao.getSzpjData(step_id, "F48D10422AFB430298B879E74920D5A7");
        String[] wrws = data.split(";");
        if (StringUtils.isEmpty(riqi)) {
            return false;
        } else {
            if (StringUtils.isEmpty(value)) {
                value = "0";
            }
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            Calendar ca = Calendar.getInstance();
            ca.setTime(sdf.parse(riqi));//实例化一个日期
            int month = ca.get(Calendar.DAY_OF_YEAR);
            BufferedWriter writer = null;
            for (int i = 0; i < wrws.length; i++) {
                String[] wrw = wrws[i].split(",");
                int index = Integer.parseInt(wrw[0].substring(3));
                try {
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file + "/JH" + index + "T.000")));
                    writer.write("COD   TIME SERIA      1            2         3        4       5       6       7       8        9        10");
                    writer.newLine();
                    for (int j = 1; j <= 366; j++) {
                        if (j == month) {
                            writer.write("\t" + j + "\t" + value + "      0.00      0.00     0.00    0.00    0.00    0.00     0.00     0.00     0.00");
                        } else {
                            writer.write("\t" + j + "\t" + "0.00         0.00      0.00     0.00    0.00    0.00    0.00     0.00     0.00     0.00");
                        }
                        writer.newLine();
                    }
                    writer.flush();
                    writer.close();
                    return true;
                } catch (Exception e) {
                    e.printStackTrace();
                    throw e;
                }
            }
        }
        return false;
    }

    @Override
    public List<Map<String, String>> getWuRanWuList(String planId) throws SQLException {
        return dao.getWuRanWuList(planId);
    }

    @Override
    public String getJianCeItemId(String name) throws SQLException {
        return dao.getJianCeItemId(name);
    }

    @Override
    public void updatePlanState(String plan_id, int i) throws SQLException {
        dao.updatePlanState(plan_id, i);
    }

    @Override
    public List<Map<String, Object>> waterHDSsSource(String planId, String juliId, String jisuanId, String item_id, String name) throws Exception {
        List<Map<String, Object>> rsList = new ArrayList<Map<String, Object>>();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String stepId = dao.getStepByPlan(planId);

        String shijianjiange = dao.getSzpjData(stepId, "A794B4B4344743FB82F3B06777EEC67B");
        if ("SECD".equals(shijianjiange)) {
            shijianjiange = "秒";
        } else if ("HOUR".equals(shijianjiange)) {
            shijianjiange = "小时";
        } else if ("DAYS".equals(shijianjiange)) {
            shijianjiange = "天";
        }
        int buchang = dao.getSzpjData(stepId, "3F12ADDAC1574137AE7D8379C8C85813") == null ? 0 : Integer.parseInt(dao.getSzpjData(stepId, "3F12ADDAC1574137AE7D8379C8C85813"));

        String start = dao.getSzpjData(stepId, "4D948A512FE1464283F8BB8099D1603E");
        String zongbuchang = dao.getSzpjData(stepId, "9A4466629D6449CBAB736EC6DAD01FFD");
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(sdf.parse(start));
        calendar.add(Calendar.DATE, Integer.valueOf(zongbuchang) - 1);
        String end = sdf.format(calendar.getTime());
        try {
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                int year = sdlDao.getPlanYear(planId);
                start = year + "-01-01";
                end = year + "-12-31";
            }

            String currentMonth = null;
            String lastMonth = null;
            List<Map<String, String>> result = null;

            Map<String, String> rsMap = null;
            Map<String, Object> item = new HashMap<String, Object>();
            Map<String, String> map = sdlDao.waterHDMetaData(planId);
            List<Map<String, String>> list = dao.waterHDSsSource(stepId, juliId, jisuanId, item_id, start, end);
            Map<String, String> listMap = null;
            int monthIndex = 0;
            double maxValue = Double.MIN_VALUE;
            String maxDmName = null;
            String maxOjbId = null;

            Map<String, Object> distance = new HashMap<String, Object>();
            Map<String, String> item1 = new HashMap<String, String>();
            item1.put("name", "本溪市");
            item1.put("begin", "60.33");
            item1.put("end", "71.83");
            item1.put("color", "0xF0F0F0");
            distance.put("x_1", item1);
            item1 = new HashMap<String, String>();
            item1.put("name", "辽阳市");
            item1.put("begin", "141.1");
            item1.put("end", "153.7");
            item1.put("color", "0xF0F0F0");
            distance.put("x_2", item1);
            item.put("x-distance", distance);
            item.put("x_total", "213.45");
            rsList.add(item);
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
                    if ("秒".equals(shijianjiange)) {
                        item.put("label", getFormatDate(monthIndex * buchang) + "后");
                    } else {
                        item.put("label", (monthIndex * buchang) + shijianjiange + "后");
                    }
                    item.put("color", randomColor());
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    result.add(rsMap);
                } else if (lastMonth.equals(currentMonth)) {
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    if (Double.parseDouble(listMap.get("value")) > maxValue) {
                        maxValue = Double.parseDouble(listMap.get("value"));
                        maxDmName = listMap.get("dmName");
                        maxOjbId = listMap.get("super_obj_id");
                    }

                    result.add(rsMap);
                } else {
                    lastMonth = currentMonth;
                    item.put("nodeList", result);
                    item.put("对应浓度值", maxValue);
                    item.put("最高浓度断面名称", maxDmName);
                    item.put("super_obj_id", maxOjbId);
                    rsList.add(item);
                    monthIndex++;
                    item = new HashMap<String, Object>();
                    item.put("type", "LineChart");
                    item.put("schemeName", map.get("schemeName"));
                    item.put("schemeCode", map.get("schemeCode"));
                    item.put("sectionName", map.get("sectionName"));
                    item.put("sectionCode", map.get("sectionCode"));
                    item.put("year", map.get("year"));
                    if ("秒".equals(shijianjiange)) {
                        item.put("label", getFormatDate(monthIndex * buchang) + "后");
                    } else {
                        item.put("label", (monthIndex * buchang) + shijianjiange + "后");
                    }
                    item.put("color", randomColor());
                    item.put("xField", "distance");
                    item.put("yField", "value");
                    item.put("xFieldName", "距离");
                    item.put("yFieldName", name);
                    result = new ArrayList<Map<String, String>>();
                    rsMap = new HashMap<String, String>();
                    rsMap.put("distance", listMap.get("distance"));
                    rsMap.put("value", listMap.get("value"));
                    rsMap.put("obj_id", listMap.get("obj_id"));
                    rsMap.put("super_obj_id", listMap.get("super_obj_id"));
                    rsMap.put("空间数据id", listMap.get("空间数据id"));
                    rsMap.put("colo_value", listMap.get("colo_value"));
                    maxValue = Double.parseDouble(listMap.get("value"));
                    maxDmName = listMap.get("dmName");
                    maxOjbId = listMap.get("super_obj_id");
                    result.add(rsMap);
                }
            }
            if (result != null) {
                item.put("对应浓度值", maxValue);
                item.put("最高浓度断面名称", maxDmName);
                item.put("super_obj_id", maxOjbId);
                item.put("nodeList", result);
            }
            rsList.add(item);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
        return rsList;
    }

    @Override
    public int getPlanState(String plan_id) throws SQLException {
        return dao.getPlanState(plan_id);
    }

    @Override
    public boolean exportJHXSData(String step_id, String filePath, boolean flag) throws Exception {
        String data = dao.getSzpjData(step_id, "F48D10422AFB430298B879E74920D5A7");
        if (data != null) {
            String[] wrws = data.split(";");
            for (int i = 0; i < wrws.length; i++) {
                String[] wrw = wrws[i].split(",");
                int index = Integer.parseInt(wrw[0].substring(3));
                BufferedWriter writer = null;
                try {
                    writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(filePath + "/JH" + index + "S.001")));
                    writer.write(wrw[1] + "\t" + wrw[2]);
                    if (flag && index == 7) {
                        if (i == 0) {
                            writer.write("\t1");
                            writer.newLine();
                            writer.write("0.0, 0.0,  0.0, 0.00,  0.0, 0.0");
                        }
                    }
                    writer.newLine();
                    for (int j = 3; j <= 8; j++) {
                        writer.write(wrw[j]);
                        writer.write("\t");
                    }
                    writer.newLine();
                    writer.write("-------------------------------------------");
                } catch (Exception e) {
                    e.printStackTrace();
                    throw e;
                } finally {
                    if (writer != null)
                        writer.close();
                }

            }
            return true;
        }
        return false;
    }

    private String randomColor() {
        String[] value = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F"};
        Random random = new Random();
        return value[random.nextInt(16)] + value[random.nextInt(16)] + value[random.nextInt(16)] + value[random.nextInt(16)] + value[random.nextInt(16)] + value[random.nextInt(16)];
    }

    private String getFormatDate(int sec) {
        int m = 60;
        int h = 3600;
        int d = 3600 * 24;
        StringBuilder sb = new StringBuilder();
        if (sec / d > 0) {
            sb.append((sec / d));
            sb.append("天");
            sec = sec % d;
        }
        if (sec / h > 0) {
            sb.append((sec / h));
            sb.append("小时");
            sec = sec % h;
        }
        if (sec / m > 0) {
            sb.append((sec / m));
            sb.append("分");
            sec = sec % m;
        }
        if (sec >= 0) {
            sb.append(sec);
            sb.append("秒");
        }
        return sb.toString();
    }
}
