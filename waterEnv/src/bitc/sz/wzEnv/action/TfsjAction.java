package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.tfsj.TfsjService;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.net.URLEncoder;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/8/30.
 */
@Controller
@RequestMapping("/tfsj")
public class TfsjAction {
    @Autowired
    private TfsjService tfsjService;

    @ResponseBody
    @RequestMapping("/newWrwProperty")
    public ExtjsResponse newWrwProperty(@RequestParam String number,String itemName) throws SQLException {
        return tfsjService.newWrwProperty(number, itemName);
    }

    @ResponseBody
    @RequestMapping("/updateWrwItemNameProperty")
    public ExtjsResponse updateWrwItemNameProperty(@RequestParam String itemId,String number,String itemName) throws SQLException {
        return tfsjService.updateWrwItemNameProperty(itemId, number, itemName);
    }

    @ResponseBody
    @RequestMapping("/updateWrwProperty")
    public ExtjsResponse updateWrwProperty(@RequestParam String itemId,String number,String itemName) throws SQLException {
        return tfsjService.updateWrwProperty(itemId, number, itemName);
    }

    @ResponseBody
    @RequestMapping("/getWrwPropertyMessage")
    public Map<String,Object> getWrwPropertyMessage() throws SQLException {
        return tfsjService.getWrwPropertyMessage();
    }

    @ResponseBody
    @RequestMapping("/queryWrwMessage")
    public ExtjsStoreResponse queryWrwMessage(String keyword, PagingInfo info) throws SQLException {
        ExtjsStoreResponse response = new  ExtjsStoreResponse();
        Map map = tfsjService.queryWrwMessage(keyword,info);
        response.setRows((List)map.get("result"));
        response.setTotal((Integer) map.get("size"));
        return response;
    }

    @ResponseBody
    @RequestMapping("/getwrwQueryGridColums")
    public ExtjsResponse getwrwQueryGridColums(){
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String,Object> list = tfsjService.getwrwQueryGridColums();
            response.setSuccess(true);
            response.addAttribute("meta",list);
        } catch (SQLException e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/queryWrwDictionaryMessage")
    public List<Map<String,String>> queryWrwDictionaryMessage(@RequestParam String wrwId) throws SQLException {
        return tfsjService.queryWrwDictionaryMessage(wrwId);
    }

    @ResponseBody
    @RequestMapping("/saveWrwDictionary")
    public ExtjsResponse saveWrwDictionary(@RequestParam String wrwId , String editWrwData, String addWrwData) throws SQLException {
        return tfsjService.saveWrwDictionary(wrwId, editWrwData, addWrwData);
    }

    @ResponseBody
    @RequestMapping("/deleteWrwDictionary")
    public ExtjsResponse deleteWrwDictionary(String wrwId) throws SQLException {
        return tfsjService.deleteWrwDictionary(wrwId);
    }

    @ResponseBody
    @RequestMapping("/queryFxyDictionaryMessage")
    List<Map<String,String>> queryFxyDictionaryMessage(@RequestParam String fxyId) throws SQLException {
        return tfsjService.queryFxyDictionaryMessage(fxyId);
    }

    @ResponseBody
    @RequestMapping("/newFxyProperty")
    public ExtjsResponse newFxyProperty(@RequestParam String number,String itemName) throws SQLException {
        return tfsjService.newFxyProperty(number, itemName);
    }

    @ResponseBody
    @RequestMapping("/updateFxyProperty")
    public ExtjsResponse updateFxyProperty(@RequestParam String itemId, String startNumber, String currentNumber, String currentItemName) throws SQLException {
        return tfsjService.updateFxyProperty(itemId, startNumber, currentNumber, currentItemName);
    }

    @ResponseBody
    @RequestMapping("/getFxyPropertyMessage")
    Map<String, Object> getFxyPropertyMessage() throws SQLException {
        return tfsjService.getFxyPropertyMessage();
    }

    @ResponseBody
    @RequestMapping("/getfxyQueryGridColums")
    ExtjsResponse getfxyQueryGridColums() throws SQLException {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String,Object> list = tfsjService.getfxyQueryGridColums();
            response.setSuccess(true);
            response.addAttribute("meta",list);
        } catch (SQLException e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/queryFxyMessage")
    public ExtjsStoreResponse queryFxyMessage(String keyword,PagingInfo info) throws SQLException {
        ExtjsStoreResponse response = new  ExtjsStoreResponse();
        Map map = tfsjService.queryFxyMessage(keyword,info);
        response.setRows((List)map.get("result"));
        response.setTotal((Integer) map.get("size"));
        return response;
    }

    @ResponseBody
    @RequestMapping("/saveFxyDictionary")
    ExtjsResponse saveFxyDictionary(@RequestParam String fxyId, String coordinate, String editFxyData, String addFxyData) throws SQLException {
        return tfsjService.saveFxyDictionary(fxyId, coordinate, editFxyData, addFxyData);
    }

    @ResponseBody
    @RequestMapping("/deleteFxyDictionary")
    public ExtjsResponse deleteFxyDictionary(String fxyId) throws SQLException {
        return tfsjService.deleteFxyDictionary(fxyId);
    }

    @ResponseBody
    @RequestMapping("/getRelatedWaterBody")
    public ExtjsStoreResponse getRelatedWaterBody(@RequestParam String id) throws SQLException {
        List<String> list = tfsjService.getRelatedWaterBody(id);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveRelatedWaterBody")
    public ExtjsResponse saveFxyRelatedWaterBody(@RequestParam String id,String waterBodyIds) throws Exception {
        tfsjService.saveRelatedWaterBody(id,waterBodyIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/queryZjDictionaryMessage")
    List<Map<String,String>> queryZjDictionaryMessage(@RequestParam String zjId) throws SQLException {
        return tfsjService.queryZjDictionaryMessage(zjId);
    }

    @ResponseBody
    @RequestMapping("/newZjProperty")
    public ExtjsResponse newZjProperty(@RequestParam String number, String itemName) throws SQLException {
        return tfsjService.newZjProperty(number, itemName);
    }

    @ResponseBody
    @RequestMapping("/updateZjProperty")
    public ExtjsResponse updateZjProperty(@RequestParam String itemId, String startNumber, String currentNumber, String currentItemName) throws SQLException {
        return tfsjService.updateZjProperty(itemId, startNumber, currentNumber, currentItemName);
    }

    @ResponseBody
    @RequestMapping("/getZjPropertyMessage")
    public Map<String, Object> getZjPropertyMessage() throws SQLException {
        return tfsjService.getZjPropertyMessage();
    }

    @ResponseBody
    @RequestMapping("/getzjQueryGridColums")
    public ExtjsResponse getzjQueryGridColums() throws SQLException {
        ExtjsResponse response = new ExtjsResponse();
        try {
            Map<String,Object> list = tfsjService.getzjQueryGridColums();
            response.setSuccess(true);
            response.addAttribute("meta",list);
        } catch (SQLException e) {
            e.printStackTrace();
            response.setSuccess(false);
            response.setMessage(e.getMessage());
        }
        return response;
    }

    @ResponseBody
    @RequestMapping("/queryZjMessage")
    public ExtjsStoreResponse queryZjMessage(String keyword,PagingInfo info) throws SQLException {
        ExtjsStoreResponse response = new  ExtjsStoreResponse();
        Map map = tfsjService.queryZjMessage(keyword,info);
        response.setRows((List)map.get("result"));
        response.setTotal((Integer) map.get("size"));
        return response;
    }

    @RequestMapping("/saveZjDictionary")
    public void saveZjDictionary(String zjId, String editZjData, String addZjData,@RequestParam MultipartFile photo,HttpServletResponse response) throws SQLException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        tfsjService.saveZjDictionary(zjId, editZjData, addZjData, photo);
        PrintWriter out = response.getWriter();
        out.print("{success:true}");
    }

    @RequestMapping("/getZjPhoto")
    public void getZjPhoto(HttpServletRequest request,HttpServletResponse response) throws SQLException, IOException {
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        String zjId = request.getParameter("zjId");
        BufferedInputStream bufferedInputStream = tfsjService.getZjPhoto(zjId);
        BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(response.getOutputStream());
        if(bufferedInputStream != null){
            int i = 0;
            while ((i = bufferedInputStream.read()) != -1){
                bufferedOutputStream.write(i);
            }
            bufferedInputStream.close();
            bufferedOutputStream.flush();
            bufferedOutputStream.close();
        }
    }

    @ResponseBody
    @RequestMapping("/deleteZjDictionary")
    public ExtjsResponse deleteZjDictionary(String zjId) throws SQLException {
        return tfsjService.deleteZjDictionary(zjId);
    }

    @ResponseBody
    @RequestMapping("/queryPlanAndLawMessage")
    public ExtjsStoreResponse queryPlanAndLawMessage( String keyword,PagingInfo info,String typeId) throws SQLException {
        ExtjsStoreResponse response = new  ExtjsStoreResponse();
        Map map = tfsjService.queryPlanAndLawMessage(keyword,info,typeId);
        response.setRows((List)map.get("result"));
        response.setTotal((Integer) map.get("size"));
        return response;
    }

    @RequestMapping("/savePlanAndLaws")
    public void savePlanAndLaws(String planAndLawsId, String planAndLawsName, Long planAndLawsTime, String planAndLawsType, @RequestParam MultipartFile file,HttpServletResponse response) throws SQLException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        tfsjService.savePlanAndLaws(planAndLawsId, planAndLawsName, planAndLawsTime, planAndLawsType, file);
        PrintWriter out = response.getWriter();
        out.print("{success:true}");
    }

    @ResponseBody
    @RequestMapping("/deletePlanAndLaws")
    public ExtjsResponse deletePlanAndLaws(String planAndLawsId) throws SQLException {
        return tfsjService.deletePlanAndLaws(planAndLawsId);
    }

    @RequestMapping("/downLoadPlanAndLaws")
    public void downLoadPlanAndLaws(HttpServletRequest request,HttpServletResponse response) throws SQLException, IOException {
        String planAndLawsId = request.getParameter("planAndLawsId");
        Map<String,Object> resultMap = tfsjService.downLoadPlanAndLaws(planAndLawsId);
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        response.setContentType("application/octet-stream");
        response.setHeader("Content-Disposition", "attachment;filename="+ URLEncoder.encode((String) resultMap.get("fileName"),"UTF-8"));
        OutputStream outputStream = response.getOutputStream();
        BufferedInputStream inputStream = (BufferedInputStream) resultMap.get("file");
        int i = 0;
        while ((i = inputStream.read()) != -1){
            outputStream.write(i);
        }
        inputStream.close();
        outputStream.flush();
        outputStream.close();
    }
}
