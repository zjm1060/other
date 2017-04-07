package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.filter.WebServiceContextFilter;
import bitc.sz.wzEnv.inputview.InputViewService;
import bitc.sz.wzEnv.service.ExecuteExeSZPJModel;
import bitc.sz.wzEnv.service.ModelInput;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import bitc.sz.wzEnv.webservice.QueryService;
import com.alibaba.fastjson.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/8/15.
 */
@Controller
@RequestMapping("/inputview")
public class InputViewController {

    @Autowired
    private InputViewService service;
    @Autowired
    private ModelInput modelInput;
    @Autowired
    private QueryService queryService;
    @Autowired
    private ExecuteExeSZPJModel szpjModel;
    @ResponseBody
    @RequestMapping("/addModelPlan")
    public ExtjsResponse addModelPlan(String plan_id, String plan_name,String plan_makeTime,String plan_owner, String run_year, String run_obj_id, String run_segment, String plan_description, HttpServletRequest request) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
//            String userId = null;
//            if (request.getSession().getAttribute("currentUser") != null) {
//                userId = (String) request.getSession().getAttribute("currentUser");
//            };
            String planId = service.addModelPlan(plan_id, plan_name, plan_makeTime, plan_owner, run_year, run_obj_id, run_segment, plan_description);
            response.addAttribute("result", planId);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getHeDuanListData")
    public ExtjsStoreResponse getHeDuanListData(String water_body_id) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getHeDuanListData(water_body_id);
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/getZhaMenListDate")
    public ExtjsResponse getZhaMenListDate(String water_body_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = modelInput.getZhaMenListDate(water_body_id);
            response.addAttribute("data", list);
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/getShuiKuListDate")
    public ExtjsStoreResponse getShuiKuListDate(String water_body_id) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, Object>> list = modelInput.getShuiKuListDate(water_body_id);
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/getZhiliuListData")
    public ExtjsStoreResponse getZhiliuListData(String water_body_id) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getZhiliuListData(water_body_id);
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getBianjieListData")
    public ExtjsStoreResponse getBianjieListData(String water_body_id,String plan_id) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getBianjieListData(water_body_id, plan_id);
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getHedaoListData")
    public ExtjsStoreResponse getHedaoListData() {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getHedaoListData();
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getDuanMianListData")
    public ExtjsStoreResponse getDuanMianListData(int hedao) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getDuanMianListData(hedao);
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getZhaMenNameAndDmId")
    public ExtjsStoreResponse getZhaMenNameAndDmId() {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = modelInput.getZhaMenNameAndDmId();
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/getSzpjGridData")
    public ExtjsStoreResponse getSzpjGridData(String plan_id) {
        ExtjsStoreResponse response = new ExtjsStoreResponse();
        try {
            response.setSuccess(true);
            List<Map<String, String>> list = service.getSzpjGridData(service.getStepByPlan(plan_id, "04b71ddcb30a454ga9ede57198834243"));
            response.setRows(list);
            response.setTotal(list.size());
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/addHeWangDiXing")
    public ExtjsResponse addHeWangDiXing(String plan_id,String jsonStr) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            String step_id =service.getStepByPlan(plan_id,"73476806f527419686d18gbca8485689");
            service.addHeWangDiXing(step_id, jsonStr);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/addSdlParams")
    public ExtjsResponse addSdlParams(String plan_id ,String bianjie_name,String bianjie_type,@RequestParam MultipartFile bianjie_file,String jsonStr,
                                      HttpServletResponse httpServletResponse) {
        httpServletResponse.setContentType("Content-Type:text/html; charset=utf-8");
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            if(bianjie_name.startsWith("_")){
                bianjie_name = bianjie_name.substring(3);
            }
            if(bianjie_file != null && bianjie_file.getSize()>0){
                service.addSdlBianjieFile(plan_id, bianjie_name, bianjie_type, bianjie_file);
            }else {
                service.addSdlBianjieGridDate(plan_id, bianjie_name, bianjie_type, jsonStr);
            }
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/addChangguiData")
    public ExtjsResponse addChangguiData(String plan_id ,@RequestParam MultipartFile bianjie_file,String jsonStr,String gridData) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            String step_id =service.getStepByPlan(plan_id, "04b71ddcb30a454ga9ede57198834243");
            if(bianjie_file.getSize()>0){
                service.addChangguiData(step_id, jsonStr, bianjie_file);
            }else {
                service.addChangguiGridData(step_id, jsonStr, gridData);
            }
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/addShunshiData")
    public ExtjsResponse addShunshiData(String plan_id ,String jsonStr) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            String step_id =service.getStepByPlan(plan_id, "04b71ddcb30a454ga9ede57198834243");
            service.addShunshiData(step_id, jsonStr);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/getUpdatePlanData")
    public ExtjsResponse getUpdatePlanData(String plan_id) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            List list = service.getUpdatePlanData(plan_id);
            response.addAttribute("data", list);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("数据获取失败！");
        }
        return response;
    }
    @RequestMapping("/extjs")
    public String extjsView() {
        return "extjsView";
    }
    @ResponseBody
    @RequestMapping("/runPlan")
    public ExtjsResponse runPlan(final String plan_id) {
        ExtjsResponse response = new ExtjsResponse();
        HttpServletRequest request = WebServiceContextFilter.request.get();
        final String sdlFile =  request.getSession().getServletContext().getRealPath("/sdlFile");
        final String szpjSsPath =  request.getSession().getServletContext().getRealPath("/szpjSsFile");
        final String szpjCgPath =  request.getSession().getServletContext().getRealPath("/szpjCgFile");
        try {
            int state = szpjModel.getPlanState(plan_id);
            if(state == 1){
                throw new RuntimeException("模型正在计算中！");
            }
            response.setSuccess(true);
            new Thread ( new Runnable() {
                @Override
                public void run() {
                    try {
                        queryService.execDataModel(plan_id,sdlFile,szpjCgPath,szpjSsPath);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }).start();
            response.setMessage("启动模型成功");
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    /**
     * 于桥模型 保存数据库表 sdlm_sys_条件项目定义表中存在的项目
     * @param planId
     * @param data
     * @return
     */
    @ResponseBody
    @RequestMapping("/addKVData")
    public ExtjsResponse addKVData(String planId,String data) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String, String> map = JSONObject.parseObject(data, Map.class);
            response.setSuccess(true);
            service.addKVData(planId, map);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    /**
     * 于桥模型,aser.inp文件
     * 如果文件和表格数据同时存在，只保存文件中数据
     * @param planId
     * @param data 保存到数据库表 sdlm_sys_条件项目定义表 中存在的项目
     * @param inputFile 保存到数据库表 yqmx_biz_运算参数表
     * @return
     */
    @ResponseBody
    @RequestMapping("/addQiXiangData")
    public ExtjsResponse addQiXiangData(String planId,@RequestParam MultipartFile inputFile,String data,String gridData ) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String, String> map = JSONObject.parseObject(data, Map.class);
            service.addKVData(planId, map);
            if(inputFile.getSize()>0){
                service.addQiXiangFileData(planId,inputFile);
            }else {
                service.addQiXiangGridData(planId,gridData);
            }
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    /**
     * 于桥模型,wser.inp 文件
     * 如果文件和表格数据同时存在，只保存文件中数据
     * @param planId
     * @param inputFile 保存到数据库表 yqmx_biz_运算参数表
     * @param gridData 保存到数据库表 yqmx_biz_运算参数表
     * @return
     */
    @ResponseBody
    @RequestMapping("/addFengChangData")
    public ExtjsResponse addFengChangData(String planId,@RequestParam MultipartFile inputFile,String gridData ) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            if(inputFile.getSize()>0){
                service.addFengChangFileData(planId, inputFile);
            }else {
                service.addFengChangGridData(planId, gridData);
            }
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    /**
     * 于桥模型,tser.inp 文件
     * 如果文件和表格数据同时存在，只保存文件中数据
     * @param planId
     * @param inputFile 保存到数据库表 yqmx_biz_运算参数表
     * @param gridData 保存到数据库表 yqmx_biz_运算参数表
     * @return
     */
    @ResponseBody
    @RequestMapping("/addWenDuData")
    public ExtjsResponse addWenDuData(String planId,int index,@RequestParam MultipartFile inputFile,String gridData ) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            if(inputFile.getSize()>0){
                service.addWenDuFileData(planId, index, inputFile);
            }else {
                service.addWenDuGridData(planId, index, gridData);
            }
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    /**
     * 于桥模型,dser.inp 文件
     * 如果文件和表格数据同时存在，只保存文件中数据
     * @param planId
     * @param inputFile 保存到数据库表 yqmx_biz_运算参数表
     * @param gridData 保存到数据库表 yqmx_biz_运算参数表
     * @return
     */
    @ResponseBody
    @RequestMapping("/addWuRanWuData")
    public ExtjsResponse addWuRanWuData(String planId,int index,@RequestParam MultipartFile inputFile,String gridData ) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            if(inputFile.getSize()>0){
                service.addWuRanWuFileData(planId, index, inputFile);
            }else {
                service.addWuRanWuGridData(planId, index, gridData);
            }
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("保存失败！");
        }
        return response;
    }
    /**
     * 于桥模型,qser.inp 文件
     * 如果文件和表格数据同时存在，只保存文件中数据
     * @param planId
     * @param inputFile 保存到数据库表 yqmx_biz_运算参数表
     * @param gridData 保存到数据库表 yqmx_biz_运算参数表
     * @return
     */
    @ResponseBody
    @RequestMapping("/addLiuLiangData")
    public ExtjsResponse addLiuLiangData(String planId,int index,@RequestParam MultipartFile inputFile,String data,String gridData ) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String, String> map = JSONObject.parseObject(data, Map.class);
            service.addXuLieMappingData(planId, map,index);
            if(inputFile.getSize()>0){
                service.addLiuLiangFileData(planId, index, inputFile);
            }else {
                service.addLiuLiangGridData(planId, index, gridData);
            }
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    /**
     * 删除于桥模型的输入文件中的序列
     * @param planId 方案ID
     * @param fileId 文件ID
     * @param index   序列
     * @return
     */
    @ResponseBody
    @RequestMapping("/deleteYuQiaoInputData")
    public ExtjsResponse deleteYuQiaoInputData(String planId,String fileId,String data,int index) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String, String> map = JSONObject.parseObject(data, Map.class);
            service.deleteInputFileData(planId, fileId, index, map);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中输入框中输入的的数据
     * @param planId 方案ID
     * @param keys 参数字符串，用 # 分割
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoInputData")
    public ExtjsResponse getYuQiaoInputData(String planId,String keys) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String,String> map = service.getYuQiaoInputData(planId, keys);
            response.addAttribute("data", map);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中文件的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoQiXiangFileData")
    public ExtjsResponse getYuQiaoQiXiangFileData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<Map<String,String>> list = service.getYuQiaoQiXiangFileData(planId);
            response.addAttribute("data",list);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中文件的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoFengchangFileData")
    public ExtjsResponse getYuQiaoFengchangFileData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<Map<String,String>> list = service.getYuQiaoFengchangFileData(planId);
            response.addAttribute("data",list);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中文件的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoWenduFileData")
    public ExtjsResponse getYuQiaoWenduFileData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<List<Map<String,String>>> list = service.getYuQiaoWenduFileData(planId);
            response.addAttribute("data",list);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中文件的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoWuranwuFileData")
    public ExtjsResponse getYuQiaoWuranwuFileData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<List<Map<String,String>>> list = service.getYuQiaoWuranwuFileData(planId);
            response.addAttribute("data",list);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中文件的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoLiuliangFileData")
    public ExtjsResponse getYuQiaoLiuliangFileData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<List<Map<String,String>>> list = service.getYuQiaoLiuliangFileData(planId);
            response.addAttribute("data",list);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    /**
     * 获取桥模型的中输入框中输入的的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoXulieMappingData")
    public ExtjsResponse getYuQiaoXulieMappingData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            List<Map<String,String>> map = service.getYuQiaoXulieMappingData(planId);
            response.addAttribute("data", map);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
    @ResponseBody
    @RequestMapping("/runYuQiaoPlan")
    public ExtjsResponse runYuQiaoPlan(final String plan_id) {
        ExtjsResponse response = new ExtjsResponse();
        HttpServletRequest request = WebServiceContextFilter.request.get();
        final String yuqiaoFile =  request.getSession().getServletContext().getRealPath("/yuqiaoFile");
        try {
            int state = szpjModel.getPlanState(plan_id);
            if(state == 1){
                throw new RuntimeException("模型正在计算中！");
            }
            response.setSuccess(true);
            new Thread ( new Runnable() {
                @Override
                public void run() {
                    try {
                        queryService.execYuqiaoDataModel(plan_id, yuqiaoFile);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }).start();
            response.setMessage("启动模型成功");
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }
    /**
     * 获取桥模型的中输入框中输入的的数据
     * @param planId 方案ID
     * @return
     */
    @ResponseBody
    @RequestMapping("/getYuQiaoPlanData")
    public ExtjsResponse getYuQiaoPlanData(String planId) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String,String> map = service.getYuQiaoPlanData(planId);
            response.addAttribute("data", map);
            response.setSuccess(true);
        } catch (Exception e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage("出现异常，请重试！");
        }
        return response;
    }
}
