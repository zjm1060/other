package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import bitc.sz.wzEnv.dao.ExecuteExeSZPJDao;
import bitc.sz.wzEnv.sdl.SdlService;
import bitc.sz.wzEnv.service.ExecuteExeSDLModel;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import java.sql.SQLException;

/**
 * Created by Administrator on 2016/5/19.
 */
@Controller
@RequestMapping("/sdl")
public class SdlAction {
    @Autowired
    private SdlService sdlService;
    @Autowired
    private ExecuteExeSDLModel iExecuteExeModel;
    @Autowired
    private ExecuteExeSDLDao executeExeDao;
    @Autowired
    private ExecuteExeSZPJDao szpjDao;
    @ResponseBody
    @RequestMapping("/insertdmpm")
    public ExtjsResponse insertdmpm(@RequestParam String objid,String x,String y,String distance,String elevation,String sign,String remark) throws SQLException {
        sdlService.insertdmpm(objid,x,y,distance,elevation,sign,remark);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updatedmpm")
    public ExtjsResponse updatedmpm(@RequestParam String objid,String x,String y,String distance,String elevation,String sign,String remark,String snid) throws SQLException {
        sdlService.updatedmpm(objid, x, y, distance, elevation, sign, remark, snid);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deletedmpm")
    public ExtjsResponse deletedmpm(@RequestParam String snid,String objid) throws SQLException {
        sdlService.deletedmpm(snid, objid);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deleteDm")
    public ExtjsResponse deleteDm(@RequestParam String objid) throws SQLException {
        sdlService.deleteDm(objid);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/insertdmc")
    public ExtjsResponse insertdmc(@RequestParam String jisuanid,String hdName,String dmNumber,String cNumber,String cGao,String cWidth,String cArea,String cRadius) throws SQLException {
        sdlService.insertdmc(jisuanid, hdName, dmNumber, cNumber, cGao, cWidth, cArea, cRadius);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updatedmc")
    public ExtjsResponse updatedmc(@RequestParam String snid,String cNumber,String cGao,String cWidth,String cArea,String cRadius) throws SQLException {
        sdlService.updatedmc(snid, cNumber, cGao, cWidth, cArea, cRadius);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deletedmc")
    public ExtjsResponse deletedmc(@RequestParam String snid) throws SQLException {
        sdlService.deletedmc(snid);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/deleteTotalDmc")
    public ExtjsResponse deleteTotalDmc(@RequestParam String jisuanid,String hdName,String dmNumber) throws SQLException {
        sdlService.deleteTotalDmc(jisuanid, hdName, dmNumber);
        return new ExtjsResponse(true);
    }

    @ResponseBody
    @RequestMapping("/updateTotaldmc")
    public ExtjsResponse updateTotaldmc(@RequestParam String jisuanid,String hdName,String dmNumber,String newjisuanid,String newHdName,String newDmNumber) throws SQLException {
        sdlService.updateTotaldmc(jisuanid, hdName, dmNumber, newjisuanid, newHdName, newDmNumber);
        return new ExtjsResponse(true);
    }
    @ResponseBody
    @RequestMapping("/exportDMWaterPositionExcel/{planId}/{dmId}")
    public void exportDMWaterPositionExcel(@PathVariable("planId") String planId,@PathVariable("dmId") String dmId) throws Exception {
        iExecuteExeModel.exportDMWaterPositionExcel(planId, dmId, executeExeDao);
    }
    @ResponseBody
    @RequestMapping("/exportDMWaterFlowExcel/{planId}/{dmId}")
    public void exportDMWaterFlowExcel(@PathVariable("planId") String planId,@PathVariable("dmId") String dmId) throws Exception {
        iExecuteExeModel.exportDMWaterFlowExcel(planId, dmId, executeExeDao);
    }
    @ResponseBody
    @RequestMapping("/exportDMWaterSpeedExcel/{planId}/{dmId}")
    public void exportDMWaterSpeedExcel(@PathVariable("planId") String planId,@PathVariable("dmId") String dmId) throws Exception {
        iExecuteExeModel.exportDMWaterSpeedExcel(planId, dmId, executeExeDao);
    }
    @ResponseBody
    @RequestMapping("/exportDMWaterDepthExcel/{planId}/{dmId}")
    public void exportDMWaterDepthExcel(@PathVariable("planId") String planId,@PathVariable("dmId") String dmId) throws Exception {
        iExecuteExeModel.exportDMWaterDepthExcel(planId, dmId, executeExeDao);
    }
    @ResponseBody
    @RequestMapping("/exportHDWaterDataExcel/{planId}/{jisuanId}")
    public void exportHDWaterDataExcel(@PathVariable("planId") String planId,@PathVariable("jisuanId") String jisuanId) throws Exception {
        iExecuteExeModel.exportHDWaterDataExcel(planId, "73476806f527419686d18gbca8485689", jisuanId, executeExeDao);
    }
    @ResponseBody
    @RequestMapping("/exportHDDensitySourceExcel/{planId}")
    public void exportHDDensitySourceExcel(@PathVariable("planId") String planId) throws Exception {
        iExecuteExeModel.exportHDDensitySourceExcel(planId,executeExeDao,szpjDao);
    }
    @ResponseBody
    @RequestMapping("/exportDMDensitySourceExcel/{planId}/{dmId}")
    public void exportDMDensitySourceExcel(@PathVariable("planId") String planId,@PathVariable("dmId") String dmId) throws Exception {
        iExecuteExeModel.exportDMDensitySourceExcel(planId, "73476806f527419686d18gbca8485689", dmId, executeExeDao,szpjDao);
    }
}
