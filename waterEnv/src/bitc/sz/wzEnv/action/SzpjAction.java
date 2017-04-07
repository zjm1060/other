package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.szpj.model.SzType;
import bitc.sz.wzEnv.szpj.SzpjService;
import bitc.sz.wzEnv.szpj.model.Places;
import bitc.sz.wzEnv.szpj.model.QyPoint;
import bitc.sz.wzEnv.szpj.model.ZbType;
import bitc.sz.wzEnv.szpj.model.ZbValue;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.io.IOException;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/3/9.
 */
@Controller
@RequestMapping("/szpj")
public class SzpjAction {
    @Autowired
    private SzpjService szpjService;

    @ResponseBody
    @RequestMapping("/updateSzpj")
    public ExtjsResponse updateSzpj(@RequestParam String formid,long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String editData,String addData) throws SQLException {
        szpjService.updateSzpj(formid, gcTime, placeId, qyPoint, shuiwei, liuliang, shuiwen, qiwen, remark, editData, addData);
        return new ExtjsResponse(true);

    }

    @ResponseBody
    @RequestMapping("/deleteSzpj")
    public ExtjsResponse deleteSzpj(@RequestParam Long gcTime,String placeId,String qyPoint) throws SQLException {
        szpjService.deleteSzpj(gcTime, placeId, qyPoint);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/saveSzpj")
    public ExtjsResponse saveSzpj(@RequestParam long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData) throws IOException, SQLException {
        szpjService.saveSzpj(gcTime, placeId, qyPoint, shuiwei, liuliang, shuiwen, qiwen, remark, addData);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQyPoint")
    public List<QyPoint> getQyPoint() throws Exception {
        List<QyPoint> qyPoints = szpjService.getQyPoint();
        return qyPoints;
    }

    @ResponseBody
    @RequestMapping("/getZbValue")
    public List<ZbValue> getZbValue(@RequestParam String DOMAIN_ID,String TYPEID,String MONITORID,String MONITORSIDE,String MONITORDATE) throws Exception{
        List<ZbValue> zbValues = szpjService.getZbValue(DOMAIN_ID, TYPEID, MONITORID, MONITORSIDE, MONITORDATE);
        return zbValues;
    }

    @ResponseBody
    @RequestMapping("/getSzType")
    public Map<String ,List<SzType>> getSzType() throws SQLException {
        List<SzType> SzTypes = szpjService.getSzType();
        Map<String ,List<SzType>> map = new HashMap<String ,List<SzType>>();
        map.put("sz",SzTypes);
        return map;
    }

    @ResponseBody
    @RequestMapping("/getZbType")
    public List<ZbType> getZbType(@RequestParam String DOMAIN_ID) throws Exception{
        List<ZbType> zbTypes = szpjService.getZbType(DOMAIN_ID);
        return zbTypes;
    }

    @ResponseBody
    @RequestMapping("/getPlaces")
    public List<Places> getPlaces(@RequestParam String waterBodyId,String keywords) throws Exception {
        List<Places> places = szpjService.getPlaces(waterBodyId, keywords);
        return places;
    }

    @ResponseBody
    @RequestMapping("/getWaterBody")
    public List<Map<String, String>> getWaterBody() throws SQLException {
        return szpjService.getWaterBody();
    }

    //排污口水质监测
    @ResponseBody
    @RequestMapping("/getPwPlaces")
    public List<Places> getPwPlaces(@RequestParam String waterBodyId,String keywords) throws Exception {
        List<Places> places = szpjService.getPwPlaces(waterBodyId, keywords);
        return places;
    }

    @ResponseBody
    @RequestMapping("/savePwkSzpj")
    public ExtjsResponse savePwkSzpj(@RequestParam long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData,String paifang) throws IOException, SQLException {
        szpjService.savePwkSzpj(gcTime, placeId, qyPoint, shuiwei, liuliang, shuiwen, qiwen, remark, addData, paifang);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updatePwkSzpj")
    public ExtjsResponse updatePwkSzpj(@RequestParam String formid,long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String editData,String addData,String paifang) throws SQLException {
        szpjService.updatePwkSzpj(formid, gcTime, placeId, qyPoint, shuiwei, liuliang, shuiwen, qiwen, remark, editData, addData, paifang);
        return new ExtjsResponse(true);

    }

    //-------------------------------------------------------------------
    //污染物入河量

    @ResponseBody
    @RequestMapping("/getWrwZbValue")
    public List<Map<String,String>> getWrwZbValue(@RequestParam String MONITORID,String MONITORDATE) throws SQLException {
        return szpjService.getWrwZbValue(MONITORID, MONITORDATE);
    }

    @ResponseBody
    @RequestMapping("/saveSzy")
    public ExtjsResponse saveSzy(@RequestParam Long jcTime,String waterFunctionID,String addData) throws SQLException {
        szpjService.saveSzy(jcTime,waterFunctionID,addData);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deleteSzy")
    public ExtjsResponse deleteSzy(@RequestParam Long jcTime,String waterFunctionID) throws SQLException {
        szpjService.deleteSzy(jcTime,waterFunctionID);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updateSzy")
    public ExtjsResponse updateSzy(@RequestParam Long jcTime,String waterFunctionID,String editData,String beforeJcTime,String beforeWaterFounctionID) throws SQLException {
        szpjService.updateSzy(jcTime,waterFunctionID,editData,beforeJcTime,beforeWaterFounctionID);
        return new ExtjsResponse(true);
    }

}
