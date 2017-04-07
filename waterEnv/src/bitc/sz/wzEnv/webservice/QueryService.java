package bitc.sz.wzEnv.webservice;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import bitc.sz.wzEnv.dao.SuperMapDao;
import bitc.sz.wzEnv.dao.impl.ModelInputDaoImpl;
import bitc.sz.wzEnv.exception.NullException;
import bitc.sz.wzEnv.filter.WebServiceContextFilter;
import bitc.sz.wzEnv.model.*;
import bitc.sz.wzEnv.service.*;
import bitc.sz.wzEnv.util.CacheUtil;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.JsonUtil;
import bitc.sz.wzEnv.util.Md5Util;
import org.apache.commons.io.FileUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.servlet.http.HttpServletRequest;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by BomWu on 2015/12/9 0009.
 */
@Component("querySerivce")
public class QueryService implements IQueryService {
    private static Object lock = new Object();
    @Autowired
    private SuperMapDao superMapDao;
    @Autowired
    private ExecuteExeSDLDao executeExeDao;
    @Autowired
    private ExecuteExeSDLModel sdlModel;
    @Autowired
    private ExecuteExeSZPJModel szpjModel;
    @Autowired
    private ModelInput modelInput;
    @Autowired
    private ReservoirSZPJService reservoirSZPJService;
    @Autowired
    private EmergencyManagerService emergencyManagerService;
    @Autowired
    private ModelInputDaoImpl modelInputDao;

    private static boolean deleteDir(File dir) {
        if (dir.isDirectory()) {
            String[] children = dir.list();
            //递归删除目录中的子目录下
            for (int i = 0; i < children.length; i++) {
                boolean success = deleteDir(new File(dir, children[i]));
                if (!success) {
                    return false;
                }
            }
        }
        // 目录此时为空，可以删除
        return dir.delete();
    }

    @Override
    public String login(String userName, String password) {

        ExtjsResponse response = new ExtjsResponse();
        try {
            String userId = superMapDao.login(userName, password);
            if ("-999".equals(userId)) {
                response.setSuccess(false);
                response.setTitle("登录失败");
                response.setMessage("用户名或者密码错误！");
            } else {
                HttpServletRequest request = WebServiceContextFilter.request.get();
                request.getSession().setAttribute("currentUser", userId);
                response.setSuccess(true);
                List<WaterBody> list = superMapDao.getWaterBodyByUserID(userId);
                response.addAttribute("waterBodys", list);
                response.addAttribute("user", new IdAndName(userId, userName));
            }
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("login:------------------------------------------");
//        System.out.println("userName:" + userName);
//        System.out.println("password:" + password);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getMainTypes(String userId, String waterBodyId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<IdAndName> ids = superMapDao.getMainTypes(userId, waterBodyId);
            response.addAttribute("mainTypes", ids);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getMainTypes:------------------------------------------");
//        System.out.println("userId:" + userId);d
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    /**
     * @param typeIds 1,2,3 多个下拉框类型ID拼一起，逗号分隔
     */
    public String getTypes(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<TypeBody> list = superMapDao.getTypes(userId, waterBodyId, mainTypeId, typeIds, keyword);
            List<ShuiZiYuanBody> list1 = superMapDao.getObjs(userId, waterBodyId, mainTypeId, typeIds, keyword);
            response.addAttribute("types", list);
            response.addAttribute("targetList", list1);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getTypes:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println("mainTypeId:" + mainTypeId);
//        System.out.println("typeIds:" + typeIds);
//        System.out.println("keyword:" + keyword);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String searchTargets(String userId, String waterBodyId, String mainTypeId, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            response.addAttribute("types", null);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("searchTargets:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println("mainTypeId:" + mainTypeId);
//        System.out.println("keyword:" + keyword);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getTarget(String userId, String targetIds, String type, String water_body_id, String searchType, String period_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getTarget(userId, targetIds, type, water_body_id, searchType, period_id);
//            if(list.size()>0){
            response.addAttribute("targets", list);
//            }
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getTarget:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("type:" + type);
//        System.out.println("targetIds:" + targetIds);
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("searchType:" + searchType);
//        System.out.println("period_id:" + period_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getObjsByType(String userId, String waterBodyId, String mainTypeId, String typeIds) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<ShuiZiYuanBody> list = superMapDao.getObjsByType(userId, waterBodyId, mainTypeId, typeIds);
            response.addAttribute("objs", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getObjsByType:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println("mainTypeId:" + mainTypeId);
//        System.out.println("typeIds:" + typeIds);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getObjsBySearch(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<ShuiZiYuanBody> list = superMapDao.getObjsBySearch(userId, waterBodyId, mainTypeId, typeIds, keyword);
            response.addAttribute("objs", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getObjsBySearch:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println("mainTypeId:" + mainTypeId);
//        System.out.println("typeIds:" + typeIds);
//        System.out.println("keyword:" + keyword);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getObjs(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<ShuiZiYuanBody> list = superMapDao.getObjs(userId, waterBodyId, mainTypeId, typeIds, keyword);
            response.addAttribute("objs", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getObjs:------------------------------------------");
//        System.out.println("userId:" + userId);
//        System.out.println("waterBodyId:" + waterBodyId);
//        System.out.println("mainTypeId:" + mainTypeId);
//        System.out.println("typeIds:" + typeIds);
//        System.out.println("keyword:" + keyword);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getObjDetailInfo(String objId, String water_body_id, String searchType, String period_id) {

        HttpServletRequest request = WebServiceContextFilter.request.get();
        String path = "http://" + request.getServerName() + ":" + request.getServerPort() + request.getContextPath();
        String picDir = request.getSession().getServletContext().getRealPath("/resources/pic");
        String aviDir = request.getSession().getServletContext().getRealPath("/resources/avi");
        WebServiceObjDetailInfo response = new WebServiceObjDetailInfo();
        response.setMessage("信息查询成功。");
        response.setResponseSts(0);
        try {
            Map header = new HashMap();
            header.put("code", "name");
            header.put("colWidth", 0);
            header.put("decimalDigits", 0);
            header.put("showValue", "名称");

            Map header1 = new HashMap();
            header1.put("code", "value");
            header1.put("colWidth", 0);
            header1.put("decimalDigits", 0);
            header1.put("showValue", "内容");
            List headerList = new ArrayList();
            headerList.add(header);
            headerList.add(header1);
            // 详细信息
            Map detailList = new HashMap();
            detailList.put("detailType", "detailInfo");
            detailList.put("label", "详细信息");

            List<Map<String, String>> infoList = superMapDao.getObjDetailInfo(objId);
            detailList.put("resultInfoDetailList", infoList);
            detailList.put("resultInfoHeaderList", headerList);


            Map linkheader = new HashMap();
            linkheader.put("code", "FILE_NAME");
            linkheader.put("colWidth", 0);
            linkheader.put("decimalDigits", 0);
            linkheader.put("showValue", "资料名称");

            Map linkheader1 = new HashMap();
            linkheader1.put("code", "THEME");
            linkheader1.put("colWidth", 0);
            linkheader1.put("decimalDigits", 0);
            linkheader1.put("showValue", "资料类型");

            Map linkheader2 = new HashMap();
            linkheader2.put("code", "FILE_DESC");
            linkheader2.put("colWidth", 0);
            linkheader2.put("decimalDigits", 0);
            linkheader2.put("showValue", "资料描述");

            List linkheaderList = new ArrayList();
            linkheaderList.add(linkheader);
            linkheaderList.add(linkheader1);
            linkheaderList.add(linkheader2);

            Map linkDocList = new HashMap();
            linkDocList.put("detailType", "linkDoc");
            linkDocList.put("label", "关联资料");

            List media = new ArrayList();
            Map pic = null;
            Map picMap = new HashMap();
            getFiles(picMap, picDir, objId, path);
            Set<String> picKeys = picMap.keySet();
            for (String key : picKeys) {
                pic = new HashMap();
                pic.put("FILE_DESC", "");
                pic.put("FILE_FORMAT", key.substring(key.lastIndexOf(".") + 1));
                pic.put("FILE_NAME", key.substring(key.indexOf(objId) + objId.length()));
                pic.put("FILE_TYPE", 1);
                pic.put("PREVIEW_URL", picMap.get(key));
                pic.put("THEME", "");
                pic.put("THUMBNAILS_URL", picMap.get(key));
                media.add(pic);
            }


            Map avi = null;

            Map aviMap = new HashMap();
            getFiles(aviMap, aviDir, objId, path);
            Set<String> aviKeys = aviMap.keySet();

            for (String key : aviKeys) {
                avi = new HashMap();
                avi.put("FILE_DESC", "");
                avi.put("FILE_FORMAT", key.substring(key.lastIndexOf(".") + 1));
                avi.put("FILE_NAME", key.substring(key.indexOf(objId) + objId.length()));
                avi.put("FILE_TYPE", 2);
                avi.put("PREVIEW_URL", aviMap.get(key));
                avi.put("THEME", "");
                avi.put("THUMBNAILS_URL", aviMap.get(key));
                media.add(avi);
            }
            linkDocList.put("resultInfoDetailList", media);
            linkDocList.put("resultInfoHeaderList", linkheaderList);
            List list = new ArrayList();
            if (StringUtils.isNotEmpty(water_body_id) && StringUtils.isNotEmpty(searchType) && StringUtils.isNotEmpty(period_id)) {
                List<Map<Object, Object>> pollutionItemslist = superMapDao.getPollutionItems(objId, water_body_id, searchType, period_id);
                if (pollutionItemslist.size() > 0) {
                    //加入  主要污染指标  标签
                    Map pollutionHeader1 = new HashMap();
                    pollutionHeader1.put("code", "itemName");
                    pollutionHeader1.put("colWidth", 0);
                    pollutionHeader1.put("decimalDigits", 0);
                    pollutionHeader1.put("showValue", "指标名称");

                    Map pollutionHeader2 = new HashMap();
                    pollutionHeader2.put("code", "gcValue");
                    pollutionHeader2.put("colWidth", 0);
                    pollutionHeader2.put("decimalDigits", 0);
                    pollutionHeader2.put("showValue", "观测值");

                    Map pollutionHeader3 = new HashMap();
                    pollutionHeader3.put("code", "itemType");
                    pollutionHeader3.put("colWidth", 0);
                    pollutionHeader3.put("decimalDigits", 0);
                    pollutionHeader3.put("showValue", "水质类别");

                    Map pollutionHeader4 = new HashMap();
                    pollutionHeader4.put("code", "pollutionTimes");
                    pollutionHeader4.put("colWidth", 0);
                    pollutionHeader4.put("decimalDigits", 0);
                    pollutionHeader4.put("showValue", "超标倍数");

                    Map pollutionHeader5 = new HashMap();
                    pollutionHeader5.put("code", "waterGrade");
                    pollutionHeader5.put("colWidth", 0);
                    pollutionHeader5.put("decimalDigits", 0);
                    pollutionHeader5.put("showValue", "水质等级");

                    List pollutionHeaderList = new ArrayList();
                    pollutionHeaderList.add(pollutionHeader1);
                    pollutionHeaderList.add(pollutionHeader2);
                    pollutionHeaderList.add(pollutionHeader3);
                    pollutionHeaderList.add(pollutionHeader4);
                    pollutionHeaderList.add(pollutionHeader5);
                    // 主要污染指标
                    Map pollutionList = new HashMap();
                    pollutionList.put("detailType", "pollutionInfo");
                    pollutionList.put("label", "水质评价");


                    pollutionList.put("resultInfoDetailList", pollutionItemslist);
                    pollutionList.put("resultInfoHeaderList", pollutionHeaderList);
                    if (pollutionItemslist != null) {
                        list.add(pollutionList);
                    }
                }

            }
            list.add(detailList);
            list.add(linkDocList);
            response.setSurveyData(list);
        } catch (Exception e) {
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getObjDetailInfo:------------------------------------------");
//        System.out.println("objId:" + objId);
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("searchType:" + searchType);
//        System.out.println("period_id:" + period_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    private void getFiles(Map map, String filePath, String id, String basePath) {
        File root = new File(filePath);
        File[] files = root.listFiles();
        if (files.length > 0) {
            for (File file : files) {
                if (file.isDirectory()) {
                    getFiles(map, file.getPath(), id, basePath);
                } else {
                    if (file.getName().contains(id)) {
                        String path = file.getPath();
                        int index = path.indexOf("resources");
                        String path1 = path.substring(index).replaceAll("\\\\", "/");
                        map.put(file.getName(), basePath + "/" + path1);
                    }
                }
            }
        }
    }

    public String getSZPJDefaultObj(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            return JsonUtil.getJsonString(response);
        } else {
            try {
                response.setSuccess(true);
                List<Map<String, String>> list = superMapDao.getSZPJDefaultObj(water_body_id);
                response.addAttribute("targetList", list);
            } catch (Exception e) {
                response.setSuccess(false);
                response.setTitle("错误");
                response.setMessage("出现错误，请与管理员联系！");
                e.printStackTrace();
            }
        }
//        System.out.println("getSZPJDefaultObj:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getDefaultWaterFounc(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        } else {
            try {
                response.setSuccess(true);
                List<Map<String, String>> list = superMapDao.getDefaultWaterFounc(water_body_id);
                response.addAttribute("targetList", list);
            } catch (Exception e) {
                response.setSuccess(false);
                response.setTitle("错误");
                response.setMessage("出现错误，请与管理员联系！");
                e.printStackTrace();
            }
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZPJYearList(String water_body_id, String obj_ids) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            return JsonUtil.getJsonString(response);
        } else {
            try {
                response.setSuccess(true);
                List<Map<String, String>> list = superMapDao.getSZPJYearList(water_body_id, obj_ids);
                response.addAttribute("targetList", list);
            } catch (Exception e) {
                response.setSuccess(false);
                response.setTitle("错误");
                response.setMessage("出现错误，请与管理员联系！");
                e.printStackTrace();
            }
        }
//        System.out.println("getSZPJYearList:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("obj_ids:" + obj_ids);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZPJRiverList(String water_body_id, String p_in_year) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            return JsonUtil.getJsonString(response);
        } else {
            try {
                response.setSuccess(true);
                List<Map<String, String>> list = superMapDao.getSZPJRiverList(water_body_id, p_in_year);
                response.addAttribute("targetList", list);
            } catch (Exception e) {
                response.setSuccess(false);
                response.setTitle("错误");
                response.setMessage("出现错误，请与管理员联系！");
                e.printStackTrace();
            }
        }
//        System.out.println("getSZPJYearList:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("p_in_year:" + p_in_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZPJPeriodList(String water_body_id, String obj_ids, String year, String searchType) {
        ExtjsResponse extjsResponse = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            extjsResponse.setSuccess(false);
            extjsResponse.setTitle("错误");
            extjsResponse.setMessage("水体不能为空");
            return JsonUtil.getJsonString(extjsResponse);
        }
        if (StringUtils.isEmpty(year)) {
            extjsResponse.setSuccess(false);
            extjsResponse.setTitle("错误");
            extjsResponse.setMessage("年份不能为空");
            return JsonUtil.getJsonString(extjsResponse);
        }
        if (StringUtils.isEmpty(searchType)) {
            extjsResponse.setSuccess(false);
            extjsResponse.setTitle("错误");
            extjsResponse.setMessage("周期类型不能为空");
            return JsonUtil.getJsonString(extjsResponse);
        }

        try {
            extjsResponse.setSuccess(true);
            List<Map<String, String>> list = superMapDao.getSZPJPeriodList(water_body_id, obj_ids, year, searchType);
            extjsResponse.addAttribute("targetList", list);
        } catch (SQLException e) {
            extjsResponse.setSuccess(false);
            extjsResponse.setTitle("错误");
            extjsResponse.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getSZPJPeriodList:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("obj_ids:" + obj_ids);
//        System.out.println("year:" + year);
//        System.out.println("searchType:" + searchType);
//        System.out.println(JsonUtil.getJsonString(extjsResponse));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(extjsResponse);
    }

    @Override
    public String getSZPJRiverLevel(String water_body_id, String searchType, String period_id, String obj_ids) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(searchType)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("周期类型不能为空");
            return JsonUtil.getJsonString(response);
        }

        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = superMapDao.getSZPJRiverLevel(water_body_id, searchType, period_id, obj_ids);
            response.addAttribute("targetList", list);
        } catch (NullException e1) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("当前查询数据没有观测数据，无法查询，请重新查询！");
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getSZPJRiverLevel:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("searchType:" + searchType);
//        System.out.println("period_id:" + period_id);
//        System.out.println("obj_ids:" + obj_ids);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZPJItemValue(String water_body_id, String searchType, String period_id, String obj_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(searchType)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("周期类型不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(period_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("日期不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(obj_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("河流不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getSZPJItemValue(water_body_id, searchType, period_id, obj_id);
            response.addAttribute("targetList", list);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getSZPJItemValue:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("searchType:" + searchType);
//        System.out.println("period_id:" + period_id);
//        System.out.println("obj_id:" + obj_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZPJGgraphs(String period_id, String dmId, String itemId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = superMapDao.getSZPJGgraphs(period_id, dmId, itemId);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

//    @Override
//    public String getSzyWaterQuality(String water_body_id, String xzq_id, String year) {
//        ExtjsResponse response = new ExtjsResponse();
//        if (StringUtils.isEmpty(xzq_id)) {
//            response.setSuccess(false);
//            response.setTitle("错误");
//            response.setMessage("行政区不能为空");
//            return JsonUtil.getJsonString(response);
//        }
//        if (StringUtils.isEmpty(year)) {
//            response.setSuccess(false);
//            response.setTitle("错误");
//            response.setMessage("年份不能为空");
//            return JsonUtil.getJsonString(response);
//        }
//        try {
//            response.setSuccess(true);
//            List<Map<Object,Object>> xzqList = superMapDao.getxzqList(water_body_id, xzq_id);
//            Map<String,Object> list = superMapDao.getSzyWaterQuality(xzqList,water_body_id, xzq_id, year);
//            response.addAttribute("targetList", list);
//        } catch (SQLException e) {
//            response.setSuccess(false);
//            response.setTitle("错误");
//            response.setMessage("出现错误，请与管理员联系！");
//            e.printStackTrace();
//        }
//        return JsonUtil.getJsonString(response);
//    }

    @Override
    public String getAllXzqWaterQuality(String water_body_id, String year) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(year)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("年份不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = superMapDao.getAllXzqWaterQuality(water_body_id, year);
            response.addAttribute("targetList", list);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getAllXzqWaterQuality:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("year:" + year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String execDataModel(String plan_id, String sdlFile, String szpjCgPath, String szpjSsPath) throws Exception {
        String value = szpjModel.getModelType(plan_id);
        szpjModel.updatePlanState(plan_id, 1);
        if ("1".equals(value)) {
            execSzpjCgDataModel(plan_id, sdlFile, szpjCgPath, false);
        } else if ("2".equals(value)) {
            execSzpjSsDataModel(plan_id, sdlFile, szpjSsPath, true);
        } else {
            execSdlDataModel(plan_id, sdlFile);
        }
        return null;
    }

    @Override
    public String execSzpjCgDataModel(String plan_id, String sdlFile, String szpjPath, boolean flag) throws Exception {
        superMapDao.deleteModelData(plan_id);
        Runtime runtime = Runtime.getRuntime();
        ExtjsResponse response = new ExtjsResponse();
        String tempFileName = Md5Util.getUUID();
        String tmpDir = System.getProperty("java.io.tmpdir");
        String rootPath = tmpDir + "/" + tempFileName;
        File file = new File(rootPath + "/sdl");
        if (!file.exists()) {
            file.mkdirs();
        }

        String basePath = sdlFile;
        Properties props = System.getProperties();
        String osName = props.getProperty("os.name").toLowerCase();
        String copyFile = null;
        if (osName.indexOf("windows") >= 0) {
            copyFile = "xcopy /E " + basePath + " " + file.getPath();
        } else {
            copyFile = "cp -r " + basePath + " " + file.getPath();
        }
        BufferedReader reader0 = null;
        try {
            //复制 模型
            synchronized (lock) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            String zhiliuNames = executeExeDao.getZhiliuNames(plan_id, "31BB2E0C29864A908CACC114AC2ACBDB");
            String[] zhiliu = zhiliuNames.split(",");
            String[] zhamenNames = executeExeDao.getZhiliuNames(plan_id, "34941217879E4FC8AA5CC88BC3726A45").split(";");
            String[] zhamen = new String[zhamenNames.length];
            for (int i = 0; i < zhamenNames.length; i++) {
                zhamen[i] = zhamenNames[i].split(",")[0];
            }
            if (!sdlModel.writeTaiZiHModelInFile(plan_id, file.getPath() + "/COND", executeExeDao, zhiliu, zhamen)) {
                throw new Exception("生成水动力输入文件出错");
            }
            //执行模型
            Process proc = runtime.exec(file.getPath() + "/DSED.exe", null, file);
            proc.waitFor();

            File hydro = new File(file.getPath() + "/hydro.plt");
            File OUTQAB = new File(file.getPath() + "/OUTQAB.000");
            reader0 = new BufferedReader(new InputStreamReader(new FileInputStream(hydro)));
            String line = null;
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
            parseOutFile(hydro, OUTQAB, countNum, plan_id);
            file = new File(rootPath + "/szpj");
            if (!file.exists()) {
                file.mkdirs();
            }
            basePath = szpjPath;
            if (osName.indexOf("windows") >= 0) {
                copyFile = "xcopy /E " + basePath + " " + file.getPath();
            } else {
                copyFile = "cp -r " + basePath + " " + file.getPath();
            }

            //复制 模型
            synchronized (lock) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            FileUtils.copyFile(new File(rootPath + "/sdl/OUTQAB.000"), new File(rootPath + "/szpj/CDATA/OUTQAB.000"));
            if (!szpjModel.writeInputFile(plan_id, file.getPath() + "/CDATA", flag)) {
                throw new Exception("生成水质评价输入文件出错");
            }
//            执行模型
            proc = runtime.exec(file.getPath() + "/RiverNET_WQ.exe", null, file);
            proc.waitFor();
            File WQPLT = new File(file.getPath() + "/COUT1/WQ.PLT");
            szpjModel.readWQPLT2Database(WQPLT, plan_id);
            szpjModel.updatePlanState(plan_id, 2);
            response.setSuccess(true);
            response.setTitle("提示");
            response.setMessage("模型运行成功");
        } catch (Exception e) {
            szpjModel.updatePlanState(plan_id, 3);
            superMapDao.deleteModelData(plan_id);
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
            throw e;
        } finally {
            if (reader0 != null) {
                reader0.close();
            }
            if (new File(rootPath).exists()) {
//                deleteDir(new File(rootPath));
            }
        }
        return JsonUtil.getJsonString(response);
    }


    @Override
    public String execSzpjSsDataModel(String plan_id, String sdlFile, String szpjPath, boolean flag) throws Exception {
        superMapDao.deleteModelData(plan_id);
        Runtime runtime = Runtime.getRuntime();
        ExtjsResponse response = new ExtjsResponse();
        String tempFileName = Md5Util.getUUID();
        String tmpDir = System.getProperty("java.io.tmpdir");
        String rootPath = tmpDir + "/" + tempFileName;
        File file = new File(rootPath + "/sdl");
        if (!file.exists()) {
            file.mkdirs();
        }


        String basePath = sdlFile;
        Properties props = System.getProperties();
        String osName = props.getProperty("os.name").toLowerCase();
        String copyFile = null;
        if (osName.indexOf("windows") >= 0) {
            copyFile = "xcopy /E " + basePath + " " + file.getPath();
        } else {
            copyFile = "cp -r " + basePath + " " + file.getPath();
        }
        BufferedReader reader0 = null;
        try {
            //复制 模型
            synchronized (lock) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            String zhiliuNames = executeExeDao.getZhiliuNames(plan_id, "31BB2E0C29864A908CACC114AC2ACBDB");
            String[] zhiliu = zhiliuNames.split(",");
            String[] zhamenNames = executeExeDao.getZhiliuNames(plan_id, "34941217879E4FC8AA5CC88BC3726A45").split(";");
            String[] zhamen = new String[zhamenNames.length];
            for (int i = 0; i < zhamenNames.length; i++) {
                zhamen[i] = zhamenNames[i].split(",")[0];
            }
            if (!sdlModel.writeTaiZiHModelInFile(plan_id, file.getPath() + "/COND", executeExeDao, zhiliu, zhamen)) {
                throw new Exception("生成水动力输入文件出错");
            }
            //执行模型
            Process proc = runtime.exec(file.getPath() + "/DSED.exe", null, file);
            proc.waitFor();

            File hydro = new File(file.getPath() + "/hydro.plt");
            File OUTQAB = new File(file.getPath() + "/OUTQAB.000");
            reader0 = new BufferedReader(new InputStreamReader(new FileInputStream(hydro)));
            String line = null;
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
            parseOutFile(hydro, OUTQAB, countNum, plan_id);

            file = new File(rootPath + "/szpj");
            if (!file.exists()) {
                file.mkdirs();
            }
            basePath = szpjPath;
            if (osName.indexOf("windows") >= 0) {
                copyFile = "xcopy /E " + basePath + " " + file.getPath();
            } else {
                copyFile = "cp -r " + basePath + " " + file.getPath();
            }

            //复制 模型
            synchronized (lock) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            FileUtils.copyFile(new File(rootPath + "/sdl/OUTQAB.000"), new File(rootPath + "/szpj/CDATA/OUTQAB.000"));
            if (!szpjModel.writeInputFile(plan_id, file.getPath() + "/CDATA", flag)) {
                throw new Exception("生成水质评价输入文件出错");
            }
//            执行模型
            proc = runtime.exec(file.getPath() + "/RiverNET_WQ.exe", null, file);
            proc.waitFor();
            File WQPLT = new File(file.getPath() + "/COUT1/WQ.PLT");
            szpjModel.readWQPLT2Database(WQPLT, plan_id);
            szpjModel.updatePlanState(plan_id, 2);
            response.setSuccess(true);
            response.setTitle("提示");
            response.setMessage("模型运行成功");
        } catch (Exception e) {
            szpjModel.updatePlanState(plan_id, 3);
            superMapDao.deleteModelData(plan_id);
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
            throw e;
        } finally {
            if (reader0 != null) {
                reader0.close();
            }
            if (new File(rootPath).exists()) {
//                deleteDir(new File(rootPath));
            }
        }
        return JsonUtil.getJsonString(response);
    }

    /**
     * 先把数据模型程序拷贝到一个临时目录，然后再运行模型
     *
     * @return
     * @throws Exception
     */
    @Override
    public String execSdlDataModel(String plan_id, String sdlFile) throws Exception {
        superMapDao.deleteModelData(plan_id);
        Runtime runtime = Runtime.getRuntime();
        ExtjsResponse response = new ExtjsResponse();
        String tempFileName = Md5Util.getUUID();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = new File(tmpDir + "/" + tempFileName);
        if (!file.exists()) {
            file.mkdirs();
        }
        File hydro = null;
        File OUTQAB = null;
        String basePath = sdlFile;
        Properties props = System.getProperties();
        String osName = props.getProperty("os.name").toLowerCase();
        String copyFile = null;
        if (osName.indexOf("windows") >= 0) {
            copyFile = "xcopy /E " + basePath + " " + file.getPath();
        } else {
            copyFile = "cp -r " + basePath + " " + file.getPath();
        }
        BufferedReader reader0 = null;
        try {
            //复制 模型
            synchronized (lock) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            String zhiliuNames = executeExeDao.getZhiliuNames(plan_id, "31BB2E0C29864A908CACC114AC2ACBDB");
            String[] zhiliu = zhiliuNames.split(",");
            String[] zhamenNames = executeExeDao.getZhiliuNames(plan_id, "34941217879E4FC8AA5CC88BC3726A45").split(";");
            String[] zhamen = new String[zhamenNames.length];
            for (int i = 0; i < zhamenNames.length; i++) {
                zhamen[i] = zhamenNames[i].split(",")[0];
            }
            if (!sdlModel.writeTaiZiHModelInFile(plan_id, file.getPath() + "/COND", executeExeDao, zhiliu, zhamen)) {
                throw new Exception("生成水动力输入文件出错");
            }
            ;
            //执行模型
            Process proc = runtime.exec(file.getPath() + "/DSED.exe", null, file);
            proc.waitFor();
            hydro = new File(file.getPath() + "/hydro.plt");
            OUTQAB = new File(file.getPath() + "/OUTQAB.000");
            reader0 = new BufferedReader(new InputStreamReader(new FileInputStream(hydro)));
            String line = null;
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
            parseOutFile(hydro, OUTQAB, countNum, plan_id);
            szpjModel.updatePlanState(plan_id, 2);
            response.setSuccess(true);
            response.setTitle("提示");
            response.setMessage("模型运行成功");
        } catch (Exception e) {
            szpjModel.updatePlanState(plan_id, 3);
            superMapDao.deleteModelData(plan_id);
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        } finally {
            if (reader0 != null) {
                reader0.close();
            }
            if (file.exists()) {
//                deleteDir(file);
            }
        }
        return JsonUtil.getJsonString(response);
    }

    private void parseOutFile(File hydro, File OUTQAB, int countNum, String plan_id) throws Exception {
        String line;
        BufferedReader reader1;
        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(hydro)));
        String line1;
        String dateStr = null;
        String dateStr1 = null;
        String litem[];
        // hydro 一行的数据
        String[] strData = null;
        // OUTQAB 一天的数据
        String[] strData1 = null;
        // hydro 一天的数据
        List<String[]> strList = null;
        String step_id = sdlModel.getStepByPlanId(plan_id, executeExeDao);
        superMapDao.deleteExist(step_id);
        SimpleDateFormat dateFm = new SimpleDateFormat("yyyy.M.d");
        try {
            while ((line = reader.readLine()) != null) {
                if (line.indexOf("TITLE=") >= 0) {
                    dateStr = line.substring(26, 36);
                    strList = new ArrayList<String[]>();
                } else if (line.indexOf("VARIABLES") >= 0) {
                } else if (line.indexOf("ZONE") >= 0) {
                } else {
                    strData = new String[8];
                    //读取 hydro.plt 中的数据
                    strData[0] = line.substring(0, 5).trim();
                    strData[1] = line.substring(6, 20).trim();
                    strData[2] = line.substring(21, 30).trim();
                    strData[3] = line.substring(31, 40).trim();
                    strData[4] = line.substring(41, 50).trim();
                    strData[5] = line.substring(51, 59).trim();
                    strData[6] = line.substring(60, 69).trim();
                    strData[7] = line.substring(70, 79).trim();
                    strList.add(strData);

                    //从 hydro.plt 中获取一天多少条数据，然后在 OUTQAB.000 中一天数据的行数
                    int row = 0;
                    if (countNum % 10 == 0) {
                        row = (countNum / 10) * 3 + 1;
                    } else {
                        row = (countNum / 10 + 1) * 3 + 1;
                    }
                    if (strList.size() % countNum == 0) {
                        //记录数组的下标
                        int index = 0;
                        //记录处理的行数
                        int rowNo = 0;

                        reader1 = new BufferedReader(new InputStreamReader(new FileInputStream(OUTQAB)));
                        try {
                            while ((line1 = reader1.readLine()) != null) {
                                //每天的第一行
                                if (rowNo % row == 0) {
                                    dateStr1 = line1.substring(3, 7) + "." + line1.substring(10, 12) + "." + line1.substring(13, line1.length()).trim();
                                    strData1 = new String[countNum * 3];
                                    index = 0;
                                    rowNo = 0;
                                } else {
                                    // 每天的数据数组
                                    litem = line1.split(" ");
                                    for (String item : litem) {
                                        if (!"".equals(item.trim())) {
                                            strData1[index] = item.trim();
                                            index++;
                                        }
                                    }
                                }
                                rowNo++;
                                if (rowNo == row && dateStr != null && dateStr1 != null) {
                                    if (dateFm.parse(dateStr.replace(" ", "")).equals(dateFm.parse(dateStr1.replace(" ", "")))) {
                                        superMapDao.saveSdlmxResult1(strList, strData1, dateFm.parse(dateStr.replace(" ", "")), countNum, step_id);
                                    }
                                }
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                            throw e;
                        } finally {
                            reader1.close();
                        }
                    }
                }
            }
        } finally {
            reader.close();
        }
        // TODO 保存文件,暂时先不保存
//        superMapDao.saveSzmxResultFile(hydro,step_id);
//        superMapDao.saveSzmxResultFile(OUTQAB,step_id);
    }

    @Override
    public String getSZYYearList(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = superMapDao.getSZYYearList(water_body_id);
            response.addAttribute("targetList", list);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        System.out.println("getSZYYearList:------------------------------------------");
        System.out.println("water_body_id:" + water_body_id);
        System.out.println(JsonUtil.getJsonString(response));
        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQDataList(String water_body_id, String year) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(year)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("年份不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = superMapDao.getXZQDataList(water_body_id, year);
            response.addAttribute("targetList", list);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQDataList:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("year:" + year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQCondition(String water_body_id, String xzq_id, String year) {
        WebServiceXZQCondition response = new WebServiceXZQCondition();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        if (StringUtils.isEmpty(year)) {
            response.setMessage("年份不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            List<Map<String, Object>> list = superMapDao.getXZQCondition(water_body_id, xzq_id, year);
            response.setMessage("信息查询成功");
            response.setNodeList(list);
            Map header1 = new LinkedHashMap();
            header1.put("fieldKey", "code");
            header1.put("fieldName", "河流编码");
            header1.put("colWidth", "80");
            header1.put("isMainKey", "0");

            Map header2 = new LinkedHashMap();
            header2.put("fieldKey", "name");
            header2.put("fieldName", "河流名称");
            header2.put("colWidth", "80");
            header2.put("isMainKey", "1");

            Map header3 = new LinkedHashMap();
            header3.put("fieldKey", "totalCount");
            header3.put("fieldName", "水功能区^个数");
            header3.put("colWidth", "80");
            header3.put("isMainKey", "1");

            Map header4 = new LinkedHashMap();
            header4.put("fieldKey", "totalLength");
            header4.put("fieldName", "水功能区^总长(千米)");
            header4.put("colWidth", "80");
            header4.put("isMainKey", "1");

            Map header5 = new LinkedHashMap();
            header5.put("fieldKey", "attainedLength");
            header5.put("fieldName", "水功能区水质^达标长度(千米)");
            header5.put("colWidth", "120");
            header5.put("isMainKey", "1");

            Map header6 = new LinkedHashMap();
            header6.put("fieldKey", "lengthPercent");
            header6.put("fieldName", "水功能区水质^达标长度百分比");
            header6.put("colWidth", "140");
            header6.put("isMainKey", "1");

            Map header7 = new LinkedHashMap();
            header7.put("fieldKey", "attainedCount");
            header7.put("fieldName", "水功能区水质^达标个数");
            header7.put("colWidth", "140");
            header7.put("isMainKey", "1");

            Map header8 = new LinkedHashMap();
            header8.put("fieldKey", "attainedPercent");
            header8.put("fieldName", "水功能区水质^达标个数百分比");
            header8.put("colWidth", "140");
            header8.put("isMainKey", "1");

            Map header9 = new LinkedHashMap();
            header9.put("fieldKey", "cod");
            header9.put("fieldName", "COD入河量(t/a)");
            header9.put("colWidth", "100");
            header9.put("isMainKey", "1");

            Map header10 = new LinkedHashMap();
            header10.put("fieldKey", "limitCod");
            header10.put("fieldName", "COD入河量^限排值(t/a)");
            header10.put("colWidth", "80");
            header10.put("isMainKey", "1");

            Map header11 = new LinkedHashMap();
            header11.put("fieldKey", "inputValue");
            header11.put("fieldName", "N3H-N^入河量(t/a)");
            header11.put("colWidth", "80");
            header11.put("isMainKey", "1");

            Map header12 = new LinkedHashMap();
            header12.put("fieldKey", "limitValue");
            header12.put("fieldName", "N3H-N^限排值(t/a)");
            header12.put("colWidth", "80");
            header12.put("isMainKey", "1");

            Map header13 = new LinkedHashMap();
            header13.put("fieldKey", "x");
            header13.put("fieldName", "经度");
            header13.put("colWidth", "80");
            header13.put("isMainKey", "0");

            Map header14 = new LinkedHashMap();
            header14.put("fieldKey", "y");
            header14.put("fieldName", "纬度");
            header14.put("colWidth", "80");
            header14.put("isMainKey", "0");

            List labelList = new ArrayList();
            labelList.add(header1);
            labelList.add(header2);
            labelList.add(header3);
            labelList.add(header4);
            labelList.add(header5);
            labelList.add(header6);
            labelList.add(header7);
            labelList.add(header8);
            labelList.add(header9);
            labelList.add(header10);
            labelList.add(header11);
            labelList.add(header12);
            labelList.add(header13);
            labelList.add(header14);

            response.setPropFields(labelList);
            response.setResponseSts(0);
            response.setTotalCount(list.size());
        } catch (SQLException e) {
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQCondition:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzq_id:" + xzq_id);
//        System.out.println("year:" + year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverWaterFounctionCondition(String water_body_id, String xzq_id, String year, String riverID) {
        WebServiceRiverWaterFounctionCondition response = new WebServiceRiverWaterFounctionCondition();
        try {
            List<Map<Object, Object>> list = superMapDao.getRiverWaterFounctionCondition(water_body_id, xzq_id, year, riverID);
            response.setMessage("信息查询成功");
            response.setNodeList(list);
            Map header1 = new LinkedHashMap();
            header1.put("fieldKey", "code");
            header1.put("fieldName", "河流编码");
            header1.put("colWidth", "80");
            header1.put("isMainKey", "0");

            Map header2 = new LinkedHashMap();
            header2.put("fieldKey", "name");
            header2.put("fieldName", "河流名称");
            header2.put("colWidth", "80");
            header2.put("isMainKey", "0");

            Map header3 = new LinkedHashMap();
            header3.put("fieldKey", "funCode");
            header3.put("fieldName", "水功能区^编码");
            header3.put("colWidth", "80");
            header3.put("isMainKey", "0");

            Map header4 = new LinkedHashMap();
            header4.put("fieldKey", "funName");
            header4.put("fieldName", "水功能区名称");
            header4.put("colWidth", "120");
            header4.put("isMainKey", "1");

            Map header5 = new LinkedHashMap();
            header5.put("fieldKey", "attainedPercent");
            header5.put("fieldName", "水功能区水质^达标次数/百分比");
            header5.put("colWidth", "140");
            header5.put("isMainKey", "1");

            Map header6 = new LinkedHashMap();
            header6.put("fieldKey", "isAttained");
            header6.put("fieldName", "是否达标");
            header6.put("colWidth", "80");
            header6.put("isMainKey", "1");

            Map header7 = new LinkedHashMap();
            header7.put("fieldKey", "cod");
            header7.put("fieldName", "COD入河量(t/a)");
            header7.put("colWidth", "100");
            header7.put("isMainKey", "1");

            Map header8 = new LinkedHashMap();
            header8.put("fieldKey", "inputValue");
            header8.put("fieldName", "N3H-N入河量(t/a)");
            header8.put("colWidth", "120");
            header8.put("isMainKey", "1");

            Map header9 = new LinkedHashMap();
            header9.put("fieldKey", "monDetail");
            header9.put("fieldName", "污染物浓度^监测详情");
            header9.put("colWidth", "120");
            header9.put("isMainKey", "-1");

            Map header13 = new LinkedHashMap();
            header13.put("fieldKey", "x");
            header13.put("fieldName", "经度");
            header13.put("colWidth", "80");
            header13.put("isMainKey", "0");

            Map header14 = new LinkedHashMap();
            header14.put("fieldKey", "y");
            header14.put("fieldName", "纬度");
            header14.put("colWidth", "80");
            header14.put("isMainKey", "0");

            List labelList = new ArrayList();
            labelList.add(header1);
            labelList.add(header2);
            labelList.add(header3);
            labelList.add(header4);
            labelList.add(header5);
            labelList.add(header6);
            labelList.add(header7);
            labelList.add(header8);
            labelList.add(header9);
            labelList.add(header13);
            labelList.add(header14);

            response.setPropFields(labelList);
            response.setResponseSts(0);
            response.setTotalCount(list.size());
        } catch (SQLException e) {
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverWaterFounctionCondition:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzq_id:" + xzq_id);
//        System.out.println("year:" + year);
//        System.out.println("riverID:" + riverID);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getSZYPWKObj(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            return JsonUtil.getJsonString(response);
        } else {
            try {
                response.setSuccess(true);
                List<Map<String, String>> list = superMapDao.getSZYPWKObj(water_body_id);
                response.addAttribute("targetList", list);
            } catch (Exception e) {
                response.setSuccess(false);
                response.setTitle("错误");
                response.setMessage("出现错误，请与管理员联系！");
                e.printStackTrace();
            }
        }
//        System.out.println("getSZYPWKObj:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getWaterFounctionItemValue(String water_body_id, String year, String xzq_id, String riverID, String waterFounctionID) {
        WebServiceRiverWaterFounctionCondition response = new WebServiceRiverWaterFounctionCondition();
        try {
            List<Map<Object, Object>> list = superMapDao.getWaterFounctionItemValue(water_body_id, year, xzq_id, riverID, waterFounctionID);
//            if(list.size()>0){
            response.setMessage("信息查询成功。");
            response.setNodeList(list);
            Map header1 = new LinkedHashMap();
            header1.put("fieldKey", "code");
            header1.put("fieldName", "水功能区^编码");
            header1.put("colWidth", "80");
            header1.put("isMainKey", "0");

            Map header2 = new LinkedHashMap();
            header2.put("fieldKey", "name");
            header2.put("fieldName", "水功能区^名称");
            header2.put("colWidth", "80");
            header2.put("isMainKey", "0");

            Map header3 = new LinkedHashMap();
            header3.put("fieldKey", "monitorCount");
            header3.put("fieldName", "监测次数");
            header3.put("colWidth", "80");
            header3.put("isMainKey", "1");

            Map header4 = new LinkedHashMap();
            header4.put("fieldKey", "monitorTime");
            header4.put("fieldName", "监测时间");
            header4.put("isSortField", "true");
            header4.put("isAsc", "1");
            header4.put("colWidth", "80");
            header4.put("isMainKey", "1");

            Map header5 = new LinkedHashMap();
            header5.put("fieldKey", "codValue");
            header5.put("fieldName", "Cod浓度(mg/L)");
            header5.put("colWidth", "80");
            header5.put("isMainKey", "1");

            Map header6 = new LinkedHashMap();
            header6.put("fieldKey", "codStandardValue");
            header6.put("fieldName", "Cod浓度^标准值(mg/L)");
            header6.put("colWidth", "80");
            header6.put("isMainKey", "1");

            Map header7 = new LinkedHashMap();
            header7.put("fieldKey", "anValue");
            header7.put("fieldName", "氨氮浓度(mg/L)");
            header7.put("colWidth", "80");
            header7.put("isMainKey", "1");

            Map header8 = new LinkedHashMap();
            header8.put("fieldKey", "anStandardValue");
            header8.put("fieldName", "N3H-氨氮浓度^标准值(mg/L)");
            header8.put("colWidth", "80");
            header8.put("isMainKey", "1");

            Map header13 = new LinkedHashMap();
            header13.put("fieldKey", "x");
            header13.put("fieldName", "经度");
            header13.put("colWidth", "80");
            header13.put("isMainKey", "0");

            Map header14 = new LinkedHashMap();
            header14.put("fieldKey", "y");
            header14.put("fieldName", "纬度");
            header14.put("colWidth", "80");
            header14.put("isMainKey", "0");

            List labelList = new ArrayList();
            labelList.add(header1);
            labelList.add(header2);
            labelList.add(header3);
            labelList.add(header4);
            labelList.add(header5);
            labelList.add(header6);
            labelList.add(header7);
            labelList.add(header8);
            labelList.add(header13);
            labelList.add(header14);

            response.setPropFields(labelList);
            response.setResponseSts(0);
            response.setTotalCount(list.size());

//            }
        } catch (SQLException e) {
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionItemValue:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("year:" + year);
//        System.out.println("xzq_id:" + xzq_id);
//        System.out.println("riverID:" + riverID);
//        System.out.println("waterFounctionID:" + waterFounctionID);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getPWKDetail(String water_body_id, String year, String xzq_id, String riverID, String waterFounctionID) {
        WebServiceRiverWaterFounctionCondition response = new WebServiceRiverWaterFounctionCondition();
        try {
            List<Map<Object, Object>> list = superMapDao.getPWKDetail(water_body_id, year, xzq_id, riverID, waterFounctionID);
//            if(list.size()>0){
            response.setMessage("信息查询成功。");
            response.setNodeList(list);
            Map header1 = new LinkedHashMap();
            header1.put("fieldKey", "code");
            header1.put("fieldName", "水功能区^编码");
            header1.put("colWidth", "80");
            header1.put("isMainKey", "0");

            Map header2 = new LinkedHashMap();
            header2.put("fieldKey", "name");
            header2.put("fieldName", "水功能区^名称");
            header2.put("colWidth", "80");
            header2.put("isMainKey", "0");

            Map header3 = new LinkedHashMap();
            header3.put("fieldKey", "pollutionCode");
            header3.put("fieldName", "排污口编码");
            header3.put("colWidth", "120");
            header3.put("isMainKey", "1");

            Map header4 = new LinkedHashMap();
            header4.put("fieldKey", "pollutionName");
            header4.put("fieldName", "排污口名");
            header4.put("colWidth", "80");
            header4.put("isMainKey", "1");

            Map header5 = new LinkedHashMap();
            header5.put("fieldKey", "cod");
            header5.put("fieldName", "COD入河量(t/a)");
            header5.put("colWidth", "80");
            header5.put("isMainKey", "1");

            Map header6 = new LinkedHashMap();
            header6.put("fieldKey", "inputValue");
            header6.put("fieldName", "N3H-N入河量(t/a)");
            header6.put("colWidth", "120");
            header6.put("isMainKey", "1");

            Map header13 = new LinkedHashMap();
            header13.put("fieldKey", "x");
            header13.put("fieldName", "经度");
            header13.put("colWidth", "80");
            header13.put("isMainKey", "0");

            Map header14 = new LinkedHashMap();
            header14.put("fieldKey", "y");
            header14.put("fieldName", "纬度");
            header14.put("colWidth", "80");
            header14.put("isMainKey", "0");

            List labelList = new ArrayList();
            labelList.add(header1);
            labelList.add(header2);
            labelList.add(header3);
            labelList.add(header4);
            labelList.add(header5);
            labelList.add(header6);
            labelList.add(header13);
            labelList.add(header14);

            response.setPropFields(labelList);
            response.setResponseSts(0);
            response.setTotalCount(list.size());

//            }
        } catch (SQLException e) {
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getPWKDetail:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("year:" + year);
//        System.out.println("xzq_id:" + xzq_id);
//        System.out.println("riverID:" + riverID);
//        System.out.println("waterFounctionID:" + waterFounctionID);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQBubbleDetail(String water_body_id, String year, String xzq_id) {
        WebServiceObjDetailInfo response = new WebServiceObjDetailInfo();
        try {
            List<Map<Object, Object>> list = superMapDao.getXZQBubbleDetail(water_body_id, year, xzq_id);
            response.setMessage("信息查询成功。");
            response.setResponseSts(0);
            Map header1 = new HashMap();
            header1.put("showValue", "名称");
            header1.put("decimalDigits", 0);
            header1.put("code", "name");
            header1.put("colWidth", 0);

            Map header2 = new HashMap();
            header2.put("showValue", "评估值");
            header2.put("decimalDigits", 0);
            header2.put("code", "value");
            header2.put("colWidth", 0);

            Map header3 = new HashMap();
            header3.put("showValue", "限值");
            header3.put("decimalDigits", 0);
            header3.put("code", "standard");
            header3.put("colWidth", 0);

            List headerList = new ArrayList();
            headerList.add(header1);
            headerList.add(header2);
            headerList.add(header3);

            Map detailListMap = new HashMap();
            detailListMap.put("resultInfoHeaderList", headerList);
            detailListMap.put("detailType", "detailInfo");
            detailListMap.put("label", "详细信息");
            detailListMap.put("resultInfoDetailList", list);

            List detailList = new ArrayList();
            detailList.add(detailListMap);

            response.setSurveyData(detailList);
        } catch (SQLException e) {
            e.printStackTrace();
        }
//        System.out.println("getXZQBubbleDetail:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("year:" + year);
//        System.out.println("xzq_id:" + xzq_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getWaterFounctionWaterregimen(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = superMapDao.getWaterFounctionWaterregimen(water_body_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionWaterregimen:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String waterDMPositionData(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterDMPositionData(planId, dmId, "38d5d1f65bc24641a6010759b949473b", start, end, executeExeDao));
    }

    @Override
    public String waterDMDepthData(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterDMDepthData(planId, dmId, "ae0f48e69e0d4dc79d4a995303b4d9c9", start, end, executeExeDao));
    }

    @Override
    public String waterDMFlowData(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterDMFlowData(planId, dmId, "e6e886de83254958a024f33e010700aa", start, end, executeExeDao));
    }

    @Override
    public String waterDMSpeedData(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterDMSpeedData(planId, dmId, "6641d96650694ce2a704dd1b3f71052d", start, end, executeExeDao));
    }

    @Override
    public String waterHDPositionData(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDData(planId, "8476eb918f9743a0b3053f403bb8bb22", "38d5d1f65bc24641a6010759b949473b", start, end, executeExeDao, "水位"));
    }

    @Override
    public String waterHDDepthData(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDData(planId, "8476eb918f9743a0b3053f403bb8bb22", "ae0f48e69e0d4dc79d4a995303b4d9c9", start, end, executeExeDao, "水深"));
    }

    @Override
    public String waterHDFlowData(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDData(planId, "8476eb918f9743a0b3053f403bb8bb22", "e6e886de83254958a024f33e010700aa", start, end, executeExeDao, "流量（m³/s）"));
    }

    @Override
    public String waterHDSpeedData(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDData(planId, "8476eb918f9743a0b3053f403bb8bb22", "6641d96650694ce2a704dd1b3f71052d", start, end, executeExeDao, "流速（m/s）"));
    }


    @Override
    public String waterHDPositionDataByDay(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDDataByDay(planId, "8476eb918f9743a0b3053f403bb8bb22", "38d5d1f65bc24641a6010759b949473b", start, end, executeExeDao, "水位"));
    }

    @Override
    public String waterHDDepthDataByDay(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDDataByDay(planId, "8476eb918f9743a0b3053f403bb8bb22", "ae0f48e69e0d4dc79d4a995303b4d9c9", start, end, executeExeDao, "水深"));
    }

    @Override
    public String waterHDFlowDataByDay(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDDataByDay(planId, "8476eb918f9743a0b3053f403bb8bb22", "e6e886de83254958a024f33e010700aa", start, end, executeExeDao, "流量（m³/s）"));
    }

    @Override
    public String waterHDSpeedDataByDay(String planId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.waterHDDataByDay(planId, "8476eb918f9743a0b3053f403bb8bb22", "6641d96650694ce2a704dd1b3f71052d", start, end, executeExeDao, "流速（m/s）"));
    }

    @Override
    public String writeDMTaiZiHQ(String model_id) throws Exception {
        String zhiliuNames = "Nans,Liuh,Beis,Tang,Lanh,Wuda,Xihe,Xjia,XTan";
        String[] zhiliu = zhiliuNames.split(",");
        String zhamenNames = "赛东,威宁,小堡,姚家,溪湖,彩屯,团扇子";
        String[] zhamen = zhamenNames.split(",");

        sdlModel.writeTaiZiHModelInFile("73476806f527419686d18gbca8485689", "d:/data/aa", executeExeDao, zhiliu, zhamen);
        return null;
    }

    @Override
    public String getPlanInfoList(String keyword, String water_bady_id, String userid) throws Exception {
        return JsonUtil.getJsonString(sdlModel.getPlanInfoList(keyword, water_bady_id, userid, executeExeDao));
    }

    @Override
    public String getDmInfoList(String planId) throws Exception {
        return JsonUtil.getJsonString(sdlModel.getDmInfoList(planId, executeExeDao));
    }

    @Override
    public String deletePlanById(String planId) throws Exception {
        System.out.println("---------------------删除方案ID:" + planId);
        ExtjsResponse response = new ExtjsResponse();
        try {
            superMapDao.deleteModelData(planId);
            superMapDao.deleteInputFileById(planId);
            superMapDao.deleteInputParamsById(planId);
            superMapDao.deletePlanById(planId);
            response.setSuccess(true);
            response.addAttribute("isSuccess", true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.addAttribute("isSuccess", false);
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getTerrainProfileData(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(sdlModel.getTerrainProfileData(planId, dmId, start, end, executeExeDao));
    }

    @Override
    public String getDMExportUrlList(String planId, String dmId, String jisuanId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            HttpServletRequest request = WebServiceContextFilter.request.get();
            StringBuffer url = request.getRequestURL();
            String tempUrl = url.delete(url.length() - request.getRequestURI().length(), url.length()).toString();
            String path = request.getContextPath();
            Map<String, String> map = executeExeDao.waterDMMetaData(planId, dmId);
            Map<String, String> map1 = null;
            if ("38d5d1f65bc24641a6010759b949473b".equals(jisuanId)) {
                map1 = new HashMap();
                String positionName = map.get("schemeName") + "_" + map.get("sectionName") + "_水位结果.xls";
                map1.put("fileNm", positionName);
                map1.put("fileUrl", tempUrl + path + "/sdl/exportDMWaterPositionExcel/" + planId + "/" + dmId + ".do");
            } else if ("e6e886de83254958a024f33e010700aa".equals(jisuanId)) {
                String flowName = map.get("schemeName") + "_" + map.get("sectionName") + "_流量结果.xls";
                map1 = new HashMap();
                map1.put("fileNm", flowName);
                map1.put("fileUrl", tempUrl + path + "/sdl/exportDMWaterFlowExcel/" + planId + "/" + dmId + ".do");
            } else if ("6641d96650694ce2a704dd1b3f71052d".equals(jisuanId)) {
                String speedName = map.get("schemeName") + "_" + map.get("sectionName") + "_流速结果.xls";
                map1 = new HashMap();
                map1.put("fileNm", speedName);
                map1.put("fileUrl", tempUrl + path + "/sdl/exportDMWaterSpeedExcel/" + planId + "/" + dmId + ".do");
            } else if ("ae0f48e69e0d4dc79d4a995303b4d9c9".equals(jisuanId)) {
                String depthName = map.get("schemeName") + "_" + map.get("sectionName") + "_水深结果.xls";
                map1 = new HashMap();
                map1.put("fileNm", depthName);
                map1.put("fileUrl", tempUrl + path + "/sdl/exportDMWaterDepthExcel/" + planId + "/" + dmId + ".do");
            } else if ("nh3nandcodsource".equals(jisuanId)) {
                String Name = map.get("schemeName") + "_" + map.get("sectionName") + "_污染物结果.xls";
                map1 = new HashMap();
                map1.put("fileNm", Name);
                map1.put("fileUrl", tempUrl + path + "/sdl/exportDMDensitySourceExcel/" + planId + "/" + dmId + ".do");
            }
            response.addAttribute("url", map1);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionWaterregimen:------------------------------------------");
//        System.out.println("planId:" + planId);
//        System.out.println("dmId:" + dmId);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    /**
     * 获取河道数据的下载链接
     *
     * @param planId   方案ID
     * @param jisuanId 计算项目的ID,如果是'nh3nandcodsource',则为污染物的，uuid是其他的
     * @return
     */
    @Override
    public String getHDExportUrlList(String planId, String jisuanId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            HttpServletRequest request = WebServiceContextFilter.request.get();
            StringBuffer url = request.getRequestURL();
            String tempUrl = url.delete(url.length() - request.getRequestURI().length(), url.length()).toString();
            String path = request.getContextPath();
            Map<String, String> map = null;
            Map<String, String> map1 = executeExeDao.waterHDMetaData(planId);
            if ("nh3nandcodsource".equals(jisuanId)) {
                String Name = map1.get("schemeName") + "_" + map1.get("sectionName") + "_污染物结果.xls";
                map = new HashMap();
                map.put("fileNm", Name);
                map.put("fileUrl", tempUrl + path + "/sdl/exportHDDensitySourceExcel/" + planId + ".do");
            } else {
                String name = executeExeDao.getJisuanName(jisuanId);
                map = new HashMap();
                map.put("fileNm", map1.get("schemeName") + "_" + map1.get("sectionName") + "_" + name + "结果.xls");
                map.put("fileUrl", tempUrl + path + "/sdl/exportHDWaterDataExcel/" + planId + "/" + jisuanId + ".do");
            }
            response.addAttribute("url", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionWaterregimen:------------------------------------------");
//        System.out.println("planId:" + planId);
//        System.out.println("jisuanId:" + jisuanId);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getCompareblePlanList(String planId) {
        return JsonUtil.getJsonString(sdlModel.getCompareblePlanList(planId, executeExeDao));
    }

    /**
     * 瞬时方案中污染物浓度按月平均数据
     *
     * @param planId 方案ID
     * @param start  开始日期，可为空，为空则为方案中的年份，从1月1日开始
     * @param end    结束日期，可为空，为空则为方案中的年份，从12月31日开始
     * @return
     * @throws Exception
     */
    @Override
    public String waterDMDensitySource(String planId, String dmId, String start, String end) throws Exception {
        return JsonUtil.getJsonString(szpjModel.waterDMDensitySource(planId, dmId, start, end));
    }

    /**
     * 瞬时方案中污染物浓度按天数据
     *
     * @param planId 方案ID
     * @param start  开始日期，可为空，为空则为方案中的年份，从1月1日开始
     * @param end    结束日期，可为空，为空则为方案中的年份，从12月31日开始
     * @return
     * @throws Exception
     */
    @Override
    public String waterHDDensitySourceByDay(String planId, String start, String end) throws Exception {
        ExtjsResponse response = new ExtjsResponse();
        String value = szpjModel.getModelType(planId);
        try {
            response.setSuccess(true);
            List<Map<String, String>> wuranwus = szpjModel.getWuRanWuList(planId);
            Map[] resultMap = new Map[wuranwus.size()];
            for (int i = 0; i < wuranwus.size(); i++) {
                List<Map<String, Object>> result1 = null;
                if ("1".equals(value)) {
                    result1 = szpjModel.waterHDNH3NSourceByDay(planId, "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(i).get("id"), szpjModel.getJianCeItemId(wuranwus.get(i).get("name")), start, end, wuranwus.get(i).get("name"));
                } else if ("2".equals(value)) {
                    result1 = szpjModel.waterHDSsSource(planId, "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(i).get("id"), szpjModel.getJianCeItemId(wuranwus.get(i).get("name")), wuranwus.get(i).get("name"));
                }
                Map<String, Object> map1 = new HashMap<>();
                map1.put("pollutionType", wuranwus.get(i).get("name"));
                map1.put("result", result1);
                resultMap[i] = map1;
            }
            List<Map<String, String>> list = new ArrayList<>();

            Map<String, String> map3 = new HashMap<>();
            map3.put("color", "0x3385FF");
            map3.put("label", "I类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0x4AAF48");
            map3.put("label", "II类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xEFF714");
            map3.put("label", "III类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xF7BB43");
            map3.put("label", "IV类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xFF3F00");
            map3.put("label", "V类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0x000000");
            map3.put("label", "劣V类水质");
            list.add(map3);

            response.addAttribute("targetList", resultMap);
            response.addAttribute("legends", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            response.setAttributes(null);
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String waterHDDensitySource(String planId, String start, String end) throws Exception {
        ExtjsResponse response = new ExtjsResponse();
        String value = szpjModel.getModelType(planId);
        try {
            response.setSuccess(true);
            List<Map<String, String>> wuranwus = szpjModel.getWuRanWuList(planId);
            Map[] resultMap = new Map[wuranwus.size()];
            for (int i = 0; i < wuranwus.size(); i++) {
                List<Map<String, Object>> result1 = null;
                if ("1".equals(value)) {
                    result1 = szpjModel.waterHDNH3NSource(planId, "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(i).get("id"), szpjModel.getJianCeItemId(wuranwus.get(i).get("name")), start, end, wuranwus.get(i).get("name"));
                } else if ("2".equals(value)) {
                    result1 = szpjModel.waterHDSsSource(planId, "A8E50747D5104EEGA0D6DE52CE82A15A", wuranwus.get(i).get("id"), szpjModel.getJianCeItemId(wuranwus.get(i).get("name")), wuranwus.get(i).get("name"));
                }
                Map<String, Object> map1 = new HashMap<>();
                map1.put("pollutionType", wuranwus.get(i).get("name"));
                map1.put("result", result1);
                resultMap[i] = map1;
            }
            List<Map<String, String>> list = new ArrayList<>();

            Map<String, String> map3 = new HashMap<>();
            map3.put("color", "0x3385FF");
            map3.put("label", "I类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0x4AAF48");
            map3.put("label", "II类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xEFF714");
            map3.put("label", "III类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xF7BB43");
            map3.put("label", "IV类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0xFF3F00");
            map3.put("label", "V类水质");
            list.add(map3);

            map3 = new HashMap<>();
            map3.put("color", "0x000000");
            map3.put("label", "劣V类水质");
            list.add(map3);

            response.addAttribute("targetList", resultMap);
            response.addAttribute("legends", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            response.setAttributes(null);
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQResultChart(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getXZQResultChart(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQResultChart:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQPollutionQuantity(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getXZQPollutionQuantity(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQPollutionQuantity:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverPollutionQuantity(String water_body_id, String xzqid, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getRiverPollutionQuantity(water_body_id, xzqid, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverPollutionQuantity:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzqid:" + xzqid);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getWaterFounctionPollutionQuantity(String water_body_id, String xzqid, String riverID, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getWaterFounctionPollutionQuantity(water_body_id, xzqid, riverID, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionPollutionQuantity:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzqid:" + xzqid);
//        System.out.println("riverID:" + riverID);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQPollutionExcessive(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getXZQPollutionExcessive(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQPollutionExcessive:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverPollutionExcessive(String water_body_id, String xzqid, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getRiverPollutionExcessive(water_body_id, xzqid, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverPollutionExcessive:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzqid:" + xzqid);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQWaterRate(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getXZQWaterRate(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQWaterRate:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverWaterRate(String water_body_id, String xzqid, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getRiverWaterRate(water_body_id, xzqid, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverWaterRate:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzqid:" + xzqid);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getXZQWaterRateExcessive(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getXZQWaterRateExcessive(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getXZQWaterRateExcessive:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverWaterRateExcessive(String water_body_id, String xzqid, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> list = superMapDao.getRiverWaterRateExcessive(water_body_id, xzqid, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverWaterRateExcessive:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("xzqid:" + xzqid);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getZhiliuListData(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getZhiliuListData(water_body_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getHeDuanListData(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getHeDuanListData(water_body_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getZhaMenListDate(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = modelInput.getZhaMenListDate(water_body_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    //    public String getBianJieDate(){
//        ExtjsResponse response = new ExtjsResponse();
//        try {
//            response.setSuccess(true);
//            List<String> list = modelInput.getBianJieDate();
//            response.addAttribute("targetList", list);
//        } catch (Exception e) {
//            response.setSuccess(false);
//            response.setTitle("错误");
//            response.setMessage("出现错误，请与管理员联系！");
//            e.printStackTrace();
//        }
//        return JsonUtil.getJsonString(response);
//    }
    @Override
    public String getShuiKuListDate(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = modelInput.getShuiKuListDate(water_body_id);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getRiverSZPJCount(String water_body_id, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = superMapDao.getRiverSZPJCount(water_body_id, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getRiverSZPJCount:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getWaterFounctionSZPJCount(String water_body_id, String riverID, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = superMapDao.getWaterFounctionSZPJCount(water_body_id, riverID, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getWaterFounctionSZPJCount:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("riverID:" + riverID);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getWaterFounctionSZPJCountDetail(String water_body_id, String riverId, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = superMapDao.getWaterFounctionSZPJCountDetail(water_body_id, riverId, begin_year, end_year);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getDMSZPJCount(String water_body_id, String waterFounctionID, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = superMapDao.getDMSZPJCount(water_body_id, waterFounctionID, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getDMSZPJCount:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("waterFounctionID:" + waterFounctionID);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getDMSZPJCountDetail(String water_body_id, String waterFounctionID, String begin_year, String end_year) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> list = superMapDao.getDMSZPJCountDetail(water_body_id, waterFounctionID, begin_year, end_year);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
//        System.out.println("getDMSZPJCountDetail:------------------------------------------");
//        System.out.println("water_body_id:" + water_body_id);
//        System.out.println("dmID:" + dmID);
//        System.out.println("begin_year:" + begin_year);
//        System.out.println("end_year:" + end_year);
//        System.out.println(JsonUtil.getJsonString(response));
//        System.out.println("----------------------------------------------------------");
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getLegendByType(String type) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String,String>> list = superMapDao.getLegendByType(type);
            response.addAttribute("targetList", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

//    @Override
//    public String getSZPJNutrition(String water_body_id, String obj_id, String period_id, String searchType, String itemIDs) {
//        ExtjsResponse response = new ExtjsResponse();
//        try {
//            response.setSuccess(true);
//            Map<String, Object> map = reservoirSZPJService.getSZPJNutrition(water_body_id, obj_id, period_id, searchType, itemIDs);
//            response.addAttribute("targetList", map);
//        } catch (Exception e) {
//            response.setSuccess(false);
//            response.setTitle("错误");
//            response.setMessage("出现错误，请与管理员联系！");
//            e.printStackTrace();
//        }
//        return JsonUtil.getJsonString(response);
//    }

    @Override
    public String queryWrwMessage(String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryWrwMessage(keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryFxyMessage(String water_body_id, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryFxyMessage(water_body_id, keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryZjMessage(String water_body_id, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryZjMessage(water_body_id, keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryYuanMessage(String water_body_id, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryYuanMessage(water_body_id, keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryFaguiMessage(String water_body_id, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryFaguiMessage(water_body_id, keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryWaterPollutionEventCoordinate(String water_body_id, String keyword) {
        ExtjsResponse response = new ExtjsResponse();
        if (StringUtils.isEmpty(water_body_id)) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("水体不能为空");
            return JsonUtil.getJsonString(response);
        }
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = emergencyManagerService.queryWaterPollutionEventCoordinate(water_body_id, keyword);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryWrwMessageById(String wrwId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> map = emergencyManagerService.queryWrwMessageById(wrwId);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryFxyMessageById(String fxyId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> map = emergencyManagerService.queryFxyMessageById(fxyId);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryZjMessageById(String zjId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            HttpServletRequest request = WebServiceContextFilter.request.get();
            List<Map<String, String>> map = emergencyManagerService.queryZjMessageById(zjId, request);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryYuanMessageById(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            HttpServletRequest request = WebServiceContextFilter.request.get();
            List<Map<String, Object>> map = emergencyManagerService.queryYuanMessageById(planId, request);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryFaguiMessageById(String lawId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            HttpServletRequest request = WebServiceContextFilter.request.get();
            List<Map<String, Object>> map = emergencyManagerService.queryFaguiMessageById(lawId, request);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String saveWaterPollutionEvent(String pollutedJSON) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            emergencyManagerService.saveWaterPollutionEvent(pollutedJSON);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String deleteWaterPollutionEvent(String pollutionEventId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            emergencyManagerService.deleteWaterPollutionEvent(pollutionEventId);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String queryWaterPollutionEvent(String pollutionEventId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = emergencyManagerService.queryWaterPollutionEvent(pollutionEventId);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }


    public boolean writeEfdcFile(String planId, String fielPath) throws Exception {
        modelInput.writeEfdcFile(planId, fielPath + "/efdc.inp");
        modelInput.writeTempFile(planId, fielPath + "/temp.inp");
        modelInput.writeDyeFile(planId, fielPath + "/dye.inp");
        modelInput.writeAserFile(planId, fielPath + "/aser.inp");
        modelInput.writeWserFile(planId, fielPath + "/wser.inp");
        modelInput.writeTserFile(planId, fielPath + "/tser.inp");
        modelInput.wirteDserFile(planId, fielPath + "/dser.inp");
        modelInput.wirteQserFile(planId, fielPath + "/qser.inp");
        return true;
    }

    @Override
    public void execYuqiaoDataModel(String planId, String filePath) throws SQLException {
        szpjModel.updatePlanState(planId, 1);
        modelInputDao.deleteResult(planId);
        //删除缓存
        CacheUtil.clear();

        Runtime runtime = Runtime.getRuntime();
        ExtjsResponse response = new ExtjsResponse();
        String tempFileName = Md5Util.getUUID();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = new File(tmpDir + "/" + tempFileName);
        if (!file.exists()) {
            file.mkdirs();
        }
        File vel = null;
        File wc = null;
        File ws = null;
        Properties props = System.getProperties();
        String osName = props.getProperty("os.name").toLowerCase();
        String copyFile = null;
        if (osName.indexOf("windows") >= 0) {
            copyFile = "xcopy /E " + filePath + " " + file.getPath();
        } else {
            copyFile = "cp -r " + filePath + " " + file.getPath();
        }
        try {
            //复制 模型
            synchronized (this) {
                Process cyProc = runtime.exec(copyFile);
                cyProc.waitFor();
            }
            if (!writeEfdcFile(planId, file.getPath())) {
                throw new Exception("生成水动力输入文件出错");
            }
            //执行模型
            Process proc = runtime.exec(file.getPath() + "/Console1.exe", null, file);
            proc.waitFor();
//            File resultfile = new File(file.getPath() + "/#output/EE_VEL.OUT");
//            File resultfilecopy = new File(file.getPath()+"/#output/EE_VEL.OUT1");
//            while (!resultfile.exists()){
//                Thread.sleep(100);
//            }
//            FileInputStream fi = null;
//            FileOutputStream fo = null;
//            //因为 模型运行结束后不自动退出，为了能检测到模型运行结束，
//            // 现在用的是循环删除生成的文件，在删除前先备份，如果删除成功，证明运行完成，
//            //再把备份文件恢复回去
//            //现在只通过 生成的 EE_EVL.OUT 一个文件检测，默认这个文件验证通过，就证明程序结束了
//            while (true){
//                Thread.sleep(500);
//                fi = new FileInputStream(resultfile);
//                fo = new FileOutputStream(resultfilecopy);
//                IOUtils.copy(fi, fo);
//                fi.close();
//                fo.close();
//                if(resultfile.delete()){
//                    fi = new FileInputStream(resultfilecopy);
//                    fo = new FileOutputStream(resultfile);
//                    IOUtils.copy(fi, fo);
//                    fi.close();
//                    fo.close();
//                    resultfilecopy.delete();
//                    proc.destroy();
//                    break;
//                }
//            }
            vel = new File(file.getPath() + "/#output/EE_VEL.OUT");
            wc = new File(file.getPath() + "/#output/EE_WC.OUT");
            ws = new File(file.getPath() + "/#output/EE_WS.OUT");
            modelInputDao.importVELOutputResult(vel, planId);
            modelInputDao.importWCOutputResult(wc, planId);
            modelInputDao.importWSOutputResult(ws, planId);
            szpjModel.updatePlanState(planId, 2);
            response.setSuccess(true);
            response.setTitle("提示");
            response.setMessage("模型运行成功");
        } catch (Exception e) {
            szpjModel.updatePlanState(planId, 3);
            modelInputDao.deleteResult(planId);
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        } finally {
            if (file.exists()) {
                deleteDir(file);
            }
        }
    }

    @Override
    public String getNutritionItems(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> map = reservoirSZPJService.getNutritionItems(water_body_id);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getReservoirSZPJNutrition(String water_body_id, String year, String itemIDs) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = reservoirSZPJService.getReservoirSZPJNutrition(water_body_id, year, itemIDs);
            response.addAttribute("targetList", map);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getReservoirSZPJYearList(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<Object, Object>> map = reservoirSZPJService.getReservoirSZPJYearList(water_body_id);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getReservoirSZPJNutritionCount(String water_body_id, String begin_year, String end_year, String itemIDs) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> map = reservoirSZPJService.getReservoirSZPJNutritionCount(water_body_id, begin_year, end_year, itemIDs);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getFlowBoundaryConditions(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getFlowBoundaryConditions(planId);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getPollutionBoundaryConditions(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getPollutionBoundaryConditions(planId);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getTemperatureBoundaryConditions(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getTemperatureBoundaryConditions(planId);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getDepthCurve(String planId, String iCode, String jCode) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getDepthCurve(planId, iCode, jCode);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getTemperatureCurve(String planId, String iCode, String jCode) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getTemperatureCurve(planId, iCode, jCode);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getPollutionCurve(String planId, String iCode, String jCode) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.getPollutionCurve(planId, iCode, jCode);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String renderDepthResult(String planId, int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.renderDepthResult(planId,month);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String renderTemperatureResult(String planId, int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.renderTemperatureResult(planId,month);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String renderConcentrationResult(String planId, int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.renderConcentrationResult(planId,month);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String renderFlowSpeedAndDirectionResult(String planId , int month) {
        ExtjsResponse response = new ExtjsResponse();
        long start = System.currentTimeMillis();
        String str = "";
        try {
            response.setSuccess(true);
            Map<String, Object> map = modelInputDao.renderFlowSpeedAndDirectionResult(planId , month);
            response.addAttribute("targetList", map);
            long time = System.currentTimeMillis();
            System.out.print("时间间隔1："+(time-start)/1000);
            str =  JsonUtil.getJsonString(response);
            long end = System.currentTimeMillis();
            System.out.print("时间间隔2："+(end-start)/1000);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return str;
    }

    @Override
    public String getWangegeData() throws SQLException {
        ExtjsResponse response = new ExtjsResponse();
        response.setSuccess(true);
        List<Map<String, String>> list = reservoirSZPJService.getWangegeData();
        response.addAttribute("riverCode", "");
        response.addAttribute("riverName", "");
        response.addAttribute("targetList", list);
        return JsonUtil.getJsonString(response);
    }

    @Override
    public String getMonth(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String,Integer> map = modelInputDao.getMonth(planId);
            response.addAttribute("targetList", map);
        } catch (SQLException e) {
            response.setSuccess(false);
            response.setTitle("错误");
            response.setMessage("出现错误，请与管理员联系！");
            e.printStackTrace();
        }
        return JsonUtil.getJsonString(response);
    }

}
