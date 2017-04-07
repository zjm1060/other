package bitc.sz.wzEnv.action;

/**
 * Created by zsz on 2015/12/2.
 */

import bitc.sz.wzEnv.log.Log;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.model.FormSetValueField;
import bitc.sz.wzEnv.report.model.Report;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import bitc.sz.wzEnv.zdst.ZdstService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

@Controller
@RequestMapping("/zdst")
public class ZdstAction {
    @Autowired
    private ReportService reportService;
    @Autowired
    private ZdstService zdstService;
    @ResponseBody
    @RequestMapping("/saveStWithgcz")
    public ExtjsResponse saveStWithgcz(@RequestParam String swczId,@RequestParam String stIds,@RequestParam String dbName) throws Exception {
        zdstService.saveStWithgcz(dbName, swczId, stIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/savePwWithgcz")
    public ExtjsResponse savePwWithgcz(@RequestParam String swczId,@RequestParam String pwIds,@RequestParam String dbName) throws Exception {
        zdstService.savePwWithgcz(dbName, swczId, pwIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveHddmWithgcz")
    public ExtjsResponse saveHddmWithgcz(@RequestParam String swczId,@RequestParam String hddmIds,@RequestParam String dbName) throws Exception {
        zdstService.saveHddmWithgcz(dbName, swczId, hddmIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveSydWithgcz")
    public ExtjsResponse saveSydWithgcz(@RequestParam String swczId,@RequestParam String sydIds,@RequestParam String dbName) throws Exception {
        zdstService.saveSydWithgcz(dbName, swczId, sydIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveSgnqWithgcz")
    public ExtjsResponse saveSgnqWithgcz(@RequestParam String swczId,@RequestParam String sgnqIds,@RequestParam String dbName) throws Exception {
        zdstService.saveSgnqWithgcz(dbName, swczId, sgnqIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveSt")
    public ExtjsResponse saveSt(@RequestParam String stId,String syIds,String dbName) throws Exception {
        zdstService.saveSt(dbName, stId, syIds);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/getStWithgcz")
    public ExtjsStoreResponse getStWithgcz(@RequestParam String swczId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getStWithgcz(dbName,swczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getStWithllb")
    public ExtjsStoreResponse getStWithllb(@RequestParam String lhbId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getStWithllb(dbName, lhbId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getSgnqWithgcz")
    public ExtjsStoreResponse getSgnqWithgcz(@RequestParam String swczId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getSgnqWithgcz(dbName, swczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getHddmWithgcz")
    public ExtjsStoreResponse getHddmWithgcz(@RequestParam String swczId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getHddmWithgcz(dbName, swczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getPwWithgcz")
    public ExtjsStoreResponse getPwWithgcz(@RequestParam String swczId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getPwWithgcz(dbName, swczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getSydWithgcz")
    public ExtjsStoreResponse getSydWithgcz(@RequestParam String swczId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getSydWithgcz(dbName, swczId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/getSt")
    public ExtjsStoreResponse getSt(@RequestParam String stId,String dbName) throws Exception {
        List<String> list = zdstService.getSt(dbName, stId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/getSwczscxm")
    public ExtjsStoreResponse getSwczscxm(@RequestParam String scxmId,@RequestParam String dbName) throws Exception {
        List<FormSetValueField> list = zdstService.getSwczscxm(dbName, scxmId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }
    @ResponseBody
    @RequestMapping("/updateHPRelatedRiver")
    public ExtjsResponse updateHPRelatedRiver(@RequestParam String reportToken ,String hupoID,String riverCode,String riverDirect,String heliuID) throws Exception {
        Report report  = reportService.getReportByName(reportToken);
        zdstService.updateHPRelatedRiver(report, hupoID, riverCode, riverDirect, heliuID);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deleteHPRelatedRiver")
    public ExtjsResponse deleteHPRelatedRiver(@RequestParam String reportToken ,String hupoID,String riverCode) throws Exception{
        Report report  = reportService.getReportByName(reportToken);
        zdstService.deleteHPRelatedRiver(report, hupoID, riverCode);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/insertHPRelatedRiver")
    public ExtjsResponse insertHPRelatedRiver(@RequestParam String reportToken ,String hupoID, String riverCode, String riverDirect) throws Exception{
        Report report  = reportService.getReportByName(reportToken);
        zdstService.insertHPRelatedRiver(report, hupoID, riverCode, riverDirect);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveStWithsdz")
    public ExtjsResponse saveStWithsdz(@RequestParam String slgcId,@RequestParam String stIds,@RequestParam String dbName) throws Exception {
        zdstService.saveStWithsdz(dbName, slgcId, stIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/saveStWithlhb")
    public ExtjsResponse saveStWithlhb(@RequestParam String lhbId,@RequestParam String stIds,@RequestParam String dbName) throws Exception {
        zdstService.saveStWithlhb(dbName, lhbId, stIds);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/getStWithsdz")
    public ExtjsStoreResponse getStWithsdz(@RequestParam String slgcId,@RequestParam String dbName) throws Exception {
        List<String> list = zdstService.getStWithsdz(dbName, slgcId);
        ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
        storeResponse.setSuccess(true);
        storeResponse.setRows(list);
        return storeResponse;
    }

    @ResponseBody
    @RequestMapping("/deleteSKRelatedRiver")
    public ExtjsResponse deleteSKRelatedRiver(@RequestParam String reportToken ,String shuikuID,String riverCode) throws Exception{
        Report report  = reportService.getReportByName(reportToken);
        zdstService.deleteSKRelatedRiver(report, shuikuID, riverCode);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/insertSKRelatedRiver")
    public ExtjsResponse insertSKRelatedRiver(@RequestParam String reportToken ,String shuikuID, String riverCode, String riverDirect) throws Exception{
        Report report  = reportService.getReportByName(reportToken);
        zdstService.insertSKRelatedRiver(report, shuikuID, riverCode, riverDirect);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updateSKRelatedRiver")
    public ExtjsResponse updateSKRelatedRiver(@RequestParam String reportToken ,String shuikuID,String riverCode,String riverDirect,String heliuID) throws Exception {
        Report report  = reportService.getReportByName(reportToken);
        zdstService.updateSKRelatedRiver(report, shuikuID, riverCode, riverDirect,heliuID);
        return new ExtjsResponse(true);
    }
}
