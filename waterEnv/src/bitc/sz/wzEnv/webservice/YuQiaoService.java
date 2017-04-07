package bitc.sz.wzEnv.webservice;

import bitc.sz.wzEnv.dao.impl.ModelInputDaoImpl;
import bitc.sz.wzEnv.inputview.InputViewService;
import bitc.sz.wzEnv.service.ExecuteExeSZPJModel;
import bitc.sz.wzEnv.service.ReservoirSZPJService;
import bitc.sz.wzEnv.service.impl.ModelInputImpl;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.JsonUtil;
import bitc.sz.wzEnv.util.Md5Util;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.io.File;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;
import java.util.Properties;

/**
 * Created by zsz on 2016/12/15.
 */
@Component("yuQiaoService")
public class YuQiaoService implements IYuQiaoService{

    @Autowired
    private ModelInputImpl modelInput;
    @Autowired
    private ModelInputDaoImpl modelInputDao;
    @Autowired
    private ExecuteExeSZPJModel szpjModel;
    @Autowired
    private ReservoirSZPJService reservoirSZPJService;
    public boolean writeEfdcFile(String planId,String fielPath) throws Exception {
        modelInput.writeEfdcFile(planId, fielPath+"/efdc.inp");
        modelInput.writeTempFile(planId, fielPath+"/temp.inp");
        modelInput.writeDyeFile(planId, fielPath+"/dye.inp");
         modelInput.writeAserFile(planId, fielPath+"/aser.inp");
         modelInput.writeWserFile(planId, fielPath+"/wser.inp");
         modelInput.writeTserFile(planId, fielPath+"/tser.inp");
         modelInput.wirteDserFile(planId, fielPath+"/dser.inp");
         modelInput.wirteQserFile(planId, fielPath+"/qser.inp");
        return true;
    }
    @Override
    public void execYuqiaoDataModel(String planId,String filePath) throws SQLException {
        szpjModel.updatePlanState(planId, 1);
        modelInputDao.deleteResult(planId);
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
            if (!writeEfdcFile(planId,file.getPath())) {
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
            modelInputDao.importVELOutputResult(vel,planId);
            modelInputDao.importWCOutputResult(wc,planId);
            modelInputDao.importWSOutputResult(ws,planId);
            szpjModel.updatePlanState(planId,2);
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
//                deleteDir(file);
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
            List<Map<Object,Object>> map = reservoirSZPJService.getReservoirSZPJYearList(water_body_id);
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
            List<Map<String,Object>> map = reservoirSZPJService.getReservoirSZPJNutritionCount(water_body_id, begin_year, end_year, itemIDs);
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
            Map<String,Object> map = modelInputDao.getFlowBoundaryConditions(planId);
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
            Map<String,Object> map = modelInputDao.getPollutionBoundaryConditions(planId);
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
            Map<String,Object> map = modelInputDao.getTemperatureBoundaryConditions(planId);
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
            Map<String,Object> map = modelInputDao.getDepthCurve(planId, iCode, jCode);
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
            Map<String,Object> map = modelInputDao.getTemperatureCurve(planId, iCode, jCode);
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
            Map<String,Object> map = modelInputDao.getPollutionCurve(planId, iCode, jCode);
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
    public String renderDepthResult(String planId,int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String,Object> map = modelInputDao.renderDepthResult(planId,month);
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
    public String renderTemperatureResult(String planId,int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String,Object> map = modelInputDao.renderTemperatureResult(planId,month);
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
    public String renderConcentrationResult(String planId,int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String,Object> map = modelInputDao.renderConcentrationResult(planId,month);
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
    public String renderFlowSpeedAndDirectionResult(String planId,int month) {
        ExtjsResponse response = new ExtjsResponse();
        try {
            response.setSuccess(true);
            Map<String,Object> map = modelInputDao.renderFlowSpeedAndDirectionResult(planId,month);
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
    public String getWangegeData() throws SQLException {
        ExtjsResponse response = new ExtjsResponse();
        response.setSuccess(true);
        List<Map<String,String>> list = reservoirSZPJService.getWangegeData();
        response.addAttribute("riverCode", "");
        response.addAttribute("riverName", "");
        response.addAttribute("targetList", list);
        return JsonUtil.getJsonString(response);
    }
}
