package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.mgmb.MgmbService;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

/**
 * Created by Administrator on 2015/12/4.
 */
@Controller
@RequestMapping("/mgmb")
public class MgmbAction {
    @Autowired
    private ReportService reportService;
    @Autowired
    private MgmbService mgmbService;

    @ResponseBody
    @RequestMapping("/saveSy")
    public ExtjsResponse saveSy(@RequestParam String sydId,String stIds, String dbName) throws Exception {
        mgmbService.saveSy(dbName, sydId, stIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getSy")
    public ExtjsStoreResponse getSy(@RequestParam String sydId, String dbName) throws Exception {
        List<String> list = mgmbService.getSy(dbName, sydId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQsk")
    public ExtjsResponse saveQsk(@RequestParam String qysczId,String qskIds, String dbName) throws Exception {
        mgmbService.saveQsk(dbName, qysczId, qskIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQsk")
    public ExtjsStoreResponse getQsk(@RequestParam String qysczId, String dbName) throws Exception {
        List<String> list = mgmbService.getQsk(dbName, qysczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQysh")
    public ExtjsResponse saveQysh(@RequestParam String qysczId,String qyshIds, String dbName) throws Exception {
        mgmbService.saveQysh(dbName, qysczId, qyshIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQysh")
    public ExtjsStoreResponse getQysh(@RequestParam String qysczId, String dbName) throws Exception {
        List<String> list = mgmbService.getQysh(dbName, qysczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQysczd")
    public ExtjsResponse saveQysczd(@RequestParam String qysczId,String qysczdIds, String dbName) throws Exception {
        mgmbService.saveQysczd(dbName, qysczId, qysczdIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQysczd")
    public ExtjsStoreResponse getQysczd(@RequestParam String qysczId, String dbName) throws Exception {
        List<String> list = mgmbService.getQysczd(dbName, qysczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQskqysh")
    public ExtjsResponse saveQskqysh(@RequestParam String qskId,String qyshIds, String dbName) throws Exception {
        mgmbService.saveQskqysh(dbName, qskId, qyshIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQskqysh")
    public ExtjsStoreResponse getQskqysh(@RequestParam String qskId, String dbName) throws Exception {
        List<String> list = mgmbService.getQskqysh(dbName, qskId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQsjqysh")
    public ExtjsResponse saveQsjqysh(@RequestParam String qsjId,String qyshIds, String dbName) throws Exception {
        mgmbService.saveQsjqysh(dbName, qsjId, qyshIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQsjqysh")
    public ExtjsStoreResponse getQsjqysh(@RequestParam String qsjId, String dbName) throws Exception {
        List<String> list = mgmbService.getQsjqysh(dbName, qsjId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/savePwkqysh")
    public ExtjsResponse savePwkqysh(@RequestParam String rhpwkId,String qyshIds, String dbName) throws Exception {
        mgmbService.savePwkqysh(dbName, rhpwkId, qyshIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getPwkqysh")
    public ExtjsStoreResponse getPwkqysh(@RequestParam String rhpwkId, String dbName) throws Exception {
        List<String> list = mgmbService.getPwkqysh(dbName, rhpwkId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/saveQysczpwk")
    public ExtjsResponse savePwk(@RequestParam String qysczId,String pwkIds, String dbName) throws Exception {
        mgmbService.saveQysczpwk(dbName, qysczId, pwkIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getQysczpwk")
    public ExtjsStoreResponse getQysczpwk(@RequestParam String qysczId, String dbName) throws Exception {
        List<String> list = mgmbService.getQysczpwk(dbName, qysczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/getDmCommonMsg")
    public ExtjsResponse getDmCommonMsg(@RequestParam String dbName,String hddmId)throws Exception{
        ExtjsResponse extjsResponse = new ExtjsResponse();
        List list = mgmbService.getDmCommonMsg(dbName,hddmId);
        extjsResponse.addAttribute("list", list);
        return extjsResponse;
    }

    @ResponseBody
    @RequestMapping("/saveSgnqSyd")
    public ExtjsResponse saveSgnqSyd(@RequestParam String sgnqId,String syIds, String dbName) throws Exception {
        mgmbService.saveSgnqSyd(dbName, sgnqId, syIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getSgnqSyd")
    public ExtjsStoreResponse getSgnqSyd(@RequestParam String sgnqId, String dbName) throws Exception {
        List<String> list = mgmbService.getSgnqSyd(dbName, sgnqId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/getDmController")
    public ExtjsResponse getDmController(@RequestParam String dbName,String hddmdm)throws Exception{
        ExtjsResponse extjsResponse = new ExtjsResponse();
        List list = mgmbService.getDmController(dbName,hddmdm);
        extjsResponse.addAttribute("controllerList", list);
        return extjsResponse;
    }

    @ResponseBody
    @RequestMapping("/getDmMonitor")
    public ExtjsResponse getDmMonitor(@RequestParam String dbName,String hddmdm)throws Exception{
        ExtjsResponse extjsResponse = new ExtjsResponse();
        List list = mgmbService.getDmMonitor(dbName, hddmdm);
        extjsResponse.addAttribute("monitorList", list);
        return extjsResponse;
    }

    @ResponseBody
    @RequestMapping("/getDmAdmin")
    public ExtjsResponse getDmAdmin(@RequestParam String dbName,String hddmdm)throws Exception{
        ExtjsResponse extjsResponse = new ExtjsResponse();
        List list = mgmbService.getDmAdmin(dbName, hddmdm);
        extjsResponse.addAttribute("adminList", list);
        return extjsResponse;
    }

}
