package bitc.sz.wzEnv.inputview;

import bitc.sz.wzEnv.dao.SuperMapDao;
import bitc.sz.wzEnv.dao.impl.ModelInputDaoImpl;
import bitc.sz.wzEnv.service.ExecuteExeSZPJModel;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.TypeReference;
import org.apache.http.NameValuePair;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.test.annotation.Rollback;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;

import javax.sql.DataSource;
import java.io.*;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by zsz on 2016/8/15.
 */
@Service
@Transactional(readOnly = false, rollbackFor = Exception.class)
public class InputViewService {
    volatile String a;
    @Autowired
    private InputViewDao dao;
    @Autowired
    private SuperMapDao superMapDao;
    @Autowired
    private ExecuteExeSZPJModel szpjModel;
    @Autowired
    private ModelInputDaoImpl modelInputDao;

    public String addModelPlan(String plan_id, String plan_name, String plan_makeTime, String creator, String run_year, String run_obj_id, String run_segment, String plan_description) throws Exception {
        if (plan_id != null && !"".equals(plan_id)) {
            //todo 修改方案参数，是否删除原先输出结果
//            superMapDao.deleteModelData(plan_id);
            return dao.updateModelPlan(plan_id, plan_name, run_year, run_segment, plan_description);
        } else {
            return dao.addModelPlan(plan_name, plan_makeTime, creator, run_year, run_obj_id, run_segment, plan_description);
        }
    }

    public void addHeWangDiXing(String step_id, String jsonStr) throws SQLException {
        dao.addHeWangDiXing(step_id, jsonStr);
    }

    public void addSdlBianjieFile(String plan_id, String bianjie_name, String bianjie_type, MultipartFile bianjie_file) throws Exception {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(bianjie_file.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<Map<String, String>> list = new ArrayList<>();
            Map<String, String> map = null;
            while ((line = reader.readLine()) != null) {
                map = new HashMap<>();
                String[] values = line.trim().split("\\s+");
                map.put("date", values[0]);
                map.put("value", values[1]);
                list.add(map);
            }
            dao.addSdlBianjieFile(plan_id, bianjie_name, bianjie_type, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addSdlBianjieGridDate(String plan_id, String bianjie_name, String bianjie_type, String jsonStr) throws Exception {
        List<Map<String, String>> list = JSONObject.parseObject(jsonStr, List.class);
        dao.addSdlBianjieFile(plan_id, bianjie_name, bianjie_type, list);
    }

    public void addChangguiData(String step_id, String jsonStr, MultipartFile bianjie_file) throws Exception {
        Map<String, String> map = JSONObject.parseObject(jsonStr, Map.class);
        dao.addChangguiBaseData(step_id, map);

        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(bianjie_file.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            reader.readLine();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.addChangguiFileData(step_id, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addChangguiGridData(String step_id, String jsonStr, String gridData) throws Exception {
        Map<String, String> map = JSONObject.parseObject(jsonStr, Map.class);
        dao.addChangguiBaseData(step_id, map);
        List<Map<String, String>> list = JSONObject.parseObject(gridData, List.class);
        dao.addChangguiGridData(step_id, list);
    }

    public void addShunshiData(String step_id, String jsonStr) throws Exception {
        Map<String, String> map = JSONObject.parseObject(jsonStr, Map.class);
        dao.addChangguiBaseData(step_id, map);
    }

    public List<Map<String, String>> getSzpjGridData(String step_id) throws Exception {
        return dao.getSzpjGridData(step_id);
    }

    public String getStepByPlan(String plan_id, String modelId) throws SQLException {
        return dao.getStepByPlan(plan_id, modelId);
    }

    public List getUpdatePlanData(String plan_id) throws Exception {
        Map<String, String> planInfo = dao.getPlanDataById(plan_id);
        Map<String, Object> hwdx = dao.getHwdxInfoByPlanId(plan_id);
        Map<String, Object> sdlParamsData = dao.getSdlParamsData(plan_id);
        Map<String, Object> szpjCgBaseData = dao.getSzpjCgBaseData(plan_id);
        Map<String, Object> szpjSsBaseData = dao.getSzpjSsBaseData(plan_id);
        List list = new ArrayList();
        list.add(planInfo);
        list.add(hwdx);
        list.add(sdlParamsData);
        list.add(szpjCgBaseData);
        list.add(szpjSsBaseData);
        list.add(szpjModel.getModelType(plan_id));
        return list;
    }

    public void addKVData(String planId, Map<String, String> map) throws Exception {
        dao.addChangguiBaseData(planId, map);
    }

    public void addQiXiangFileData(String planId, MultipartFile inputFile) throws Exception {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(inputFile.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.deleteYuQiaoInputData(planId, "51EF825A29DB47F598D77C73C03C661F", 1);
            dao.addQiXiangData(planId, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addQiXiangGridData(String planId, String gridData) throws SQLException {
        List list = JSONObject.parseObject(gridData, new TypeReference<List<String[]>>() {
        });
        dao.deleteYuQiaoInputData(planId, "51EF825A29DB47F598D77C73C03C661F", 1);
        dao.addQiXiangData(planId, list);
    }

    public void addFengChangFileData(String planId, MultipartFile inputFile) throws Exception {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(inputFile.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.deleteYuQiaoInputData(planId, "D11C972A5DCB48D2B0A4E1CDA65918E2", 1);
            dao.addFengChangData(planId, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addFengChangGridData(String planId, String gridData) throws SQLException {
        List list = JSONObject.parseObject(gridData, new TypeReference<List<String[]>>() {
        });
        dao.deleteYuQiaoInputData(planId, "D11C972A5DCB48D2B0A4E1CDA65918E2", 1);
        dao.addFengChangData(planId, list);
    }

    public void addWenDuFileData(String planId, int index, MultipartFile inputFile) throws IOException, SQLException {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(inputFile.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.deleteYuQiaoInputData(planId, "2304795F58E9444390159AC6C736B4B9", index);
            dao.addWenDuData(planId, index, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addWenDuGridData(String planId, int index, String gridData) throws SQLException {
        List list = JSONObject.parseObject(gridData, new TypeReference<List<String[]>>() {
        });
        dao.deleteYuQiaoInputData(planId, "2304795F58E9444390159AC6C736B4B9", index);
        dao.addWenDuData(planId, index, list);
    }

    public void addWuRanWuFileData(String planId, int index, MultipartFile inputFile) throws IOException, SQLException {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(inputFile.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.deleteYuQiaoInputData(planId, "79D02002047041818B2D97D5C6ECE619", index);
            dao.addWuRanWuData(planId, index, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addWuRanWuGridData(String planId, int index, String gridData) throws SQLException {
        List list = JSONObject.parseObject(gridData, new TypeReference<List<String[]>>() {
        });
        dao.deleteYuQiaoInputData(planId, "79D02002047041818B2D97D5C6ECE619", index);
        dao.addWuRanWuData(planId, index, list);
    }

    public void addLiuLiangFileData(String planId, int index, MultipartFile inputFile) throws IOException, SQLException {
        String fileName = UUID.randomUUID().toString();
        String tmpDir = System.getProperty("java.io.tmpdir");
        File file = null;
        FileOutputStream out = null;
        try {
            file = new File(tmpDir + "/" + fileName);
            out = new FileOutputStream(file);
            out.write(inputFile.getBytes());
            out.flush();
            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
            String line = null;
            List<String[]> list = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                String[] values = line.trim().split("\\s+");
                list.add(values);
            }
            dao.deleteYuQiaoInputData(planId, "22AC330DDC47456BB895782AD9D96CC6", index);
            dao.addLiuLiangData(planId, index, list);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (out != null) {
                out.close();
            }
            if (file.exists()) {
                file.delete();
            }
        }
    }

    public void addLiuLiangGridData(String planId, int index, String gridData) throws SQLException {
        List list = JSONObject.parseObject(gridData, new TypeReference<List<String[]>>() {
        });
        dao.deleteYuQiaoInputData(planId, "22AC330DDC47456BB895782AD9D96CC6", index);
        dao.addLiuLiangData(planId, index, list);
    }

    public void addXuLieMappingData(String planId, Map<String, String> map, int index) throws Exception {
        dao.deleteXuLieMappingData(planId, index);
        dao.addXuLieMappingData(planId, map);
    }

    public void deleteInputFileData(String planId, String fileId, int index, Map map) throws Exception {
        dao.deleteYuQiaoInputData(planId, fileId, index);
        dao.addChangguiBaseData(planId, map);
        if ("22AC330DDC47456BB895782AD9D96CC6".equals(fileId)) {
            dao.deleteXuLieMappingData(planId, index);
        }
    }

    public Map<String, String> getYuQiaoInputData(String planId, String keys) throws SQLException {
        return modelInputDao.getInputItemData(planId, keys.split("#"));
    }

    public List<Map<String, String>> getYuQiaoQiXiangFileData(String planId) throws SQLException {
        List<Map<String, String>> data = new ArrayList<>();
        Map<String, String> map = null;
        List<NameValuePair> taiyangfusheList = modelInputDao.getFileItemData(planId, "73920C3DAF3B4317935CAA5FF1DD46A8").get(0);
        List<NameValuePair> qiyaList = modelInputDao.getFileItemData(planId, "D19CCCB4732F46859FB6E2B0A58F3174").get(0);
        List<NameValuePair> qiwenList = modelInputDao.getFileItemData(planId, "8B15A500C6174F078EDCE9B4DE137B4A").get(0);
        List<NameValuePair> xiangduishiduList = modelInputDao.getFileItemData(planId, "6926DAEBC89B4D71883CD22A0A651C77").get(0);
        List<NameValuePair> jiangyuList = modelInputDao.getFileItemData(planId, "2A4201E041EF4EDEA66DEAAA671466E2").get(0);
        List<NameValuePair> zhengfaliangList = modelInputDao.getFileItemData(planId, "C15F02D85FA34A44A9F1EFF58D5898B2").get(0);
        List<NameValuePair> yuncengList = modelInputDao.getFileItemData(planId, "7F11E44802404EC0AF4DF1C6AC6D7CBF").get(0);
        if(qiyaList != null){
            for (int i = 0; i < qiyaList.size(); i++) {
                map = new HashMap<>();
                map.put("day", qiyaList.get(i).getName());
                map.put("aser_l_c02", qiyaList.get(i).getValue());
                map.put("aser_l_c03", qiwenList.get(i).getValue());
                map.put("aser_l_c04", xiangduishiduList.get(i).getValue());
                map.put("aser_l_c05", jiangyuList.get(i).getValue());
                map.put("aser_l_c06", zhengfaliangList.get(i).getValue());
                map.put("aser_l_c07", taiyangfusheList.get(i).getValue());
                map.put("aser_l_c08", yuncengList.get(i).getValue());
                data.add(map);
            }
        }
        return data;
    }

    public List<Map<String, String>> getYuQiaoFengchangFileData(String planId) throws SQLException {
        List<Map<String, String>> data = new ArrayList<>();
        Map<String, String> map = null;
        List<NameValuePair> fengxiang = modelInputDao.getFileItemData(planId, "E5F089CDE5504F3789F701B840A403B7").get(0);
        List<NameValuePair> fengsu = modelInputDao.getFileItemData(planId, "6A9F342796A940AC847A0BC843EB01F4").get(0);
        if(fengxiang != null){
            for (int i = 0; i < fengxiang.size(); i++) {
                map = new HashMap<>();
                map.put("day", fengxiang.get(i).getName());
                map.put("wser_l_c02", fengxiang.get(i).getValue());
//                map.put("wser_l_c03", fengsu.get(i).getValue());
                data.add(map);
            }
        }
        if(fengsu != null){
            for (int i = 0; i < fengsu.size(); i++) {
                if(i<data.size()){
                    map = data.get(i);
                    map.put("wser_l_c03", fengsu.get(i).getValue());
                }else{
                    map = new HashMap<>();
                    map.put("day", fengsu.get(i).getName());
                    map.put("wser_l_c03", fengsu.get(i).getValue());
                    data.add(map);
                }
            }
        }
        return data;
    }

    public List<List<Map<String, String>>> getYuQiaoWenduFileData(String planId) throws SQLException {
        List<List<Map<String, String>>> data = new ArrayList<>();
        List<Map<String, String>> xulie = null;
        List<Map<String, String>> temp = null;
        Map<String, String> map = null;
        List<List<NameValuePair>> wendu = modelInputDao.getFileItemData(planId, "343446D90C3144ABA493952C9975B216");
        for (int i = 0; wendu != null&&i < wendu.size(); i++) {
            xulie = new ArrayList<>();
            for(int j=0;wendu.get(i)!= null && j<wendu.get(i).size();j++){
                map = new HashMap<>();
                map.put("day", wendu.get(i).get(j).getName());
                map.put("wendu", wendu.get(i).get(j).getValue());
                xulie.add(map);
            }
            if(xulie != null)
            data.add(xulie);
        }
        return data;
    }

    public List<List<Map<String, String>>> getYuQiaoWuranwuFileData(String planId) throws SQLException {
        List<List<Map<String, String>>> data = new ArrayList<>();
        List<Map<String, String>> xulie = null;
        Map<String, String> map = null;
        List<List<NameValuePair>> wuranwu = modelInputDao.getFileItemData(planId, "EA2D29994A254C1B85C5B0C8F53A8648");
        for (int i = 0; wuranwu != null && i < wuranwu.size(); i++) {
            xulie = new ArrayList<>();
            for(int j=0;wuranwu.get(i)!= null &&j<wuranwu.get(i).size();j++){
                map = new HashMap<>();
                map.put("day", wuranwu.get(i).get(j).getName());
                map.put("wuranwu", wuranwu.get(i).get(j).getValue());
                xulie.add(map);
            }
            data.add(xulie);
        }
        return data;
    }

    public List<List<Map<String, String>>> getYuQiaoLiuliangFileData(String planId) throws SQLException {
        List<List<Map<String, String>>> data = new ArrayList<>();
        List<Map<String, String>> xulie = null;
        Map<String, String> map = null;
        List<List<NameValuePair>> liuliang  = modelInputDao.getFileItemData(planId, "24D8C75DC4024CA78213956FE33F01BC");
        for (int i = 0; liuliang!= null && i < liuliang.size(); i++) {
            xulie = new ArrayList<>();
            for(int j=0;liuliang.get(i)!= null && j<liuliang.get(i).size();j++){
                map = new HashMap<>();
                map.put("day", liuliang.get(i).get(j).getName());
                map.put("liuliang", liuliang.get(i).get(j).getValue());
                xulie.add(map);
            }
            data.add(xulie);
        }
        return data;
    }

    public List<Map<String, String>> getYuQiaoXulieMappingData(String planId) throws SQLException {
        return modelInputDao.getXulieMapping(planId);
    }

    public Map<String, String> getYuQiaoPlanData(String planId) throws SQLException {
        return dao.getPlanDataById(planId);
    }
}
