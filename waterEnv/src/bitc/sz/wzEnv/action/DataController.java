package bitc.sz.wzEnv.action;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import bitc.sz.wzEnv.model.Memory;
import bitc.sz.wzEnv.service.DataService;
import bitc.sz.wzEnv.util.ExtjsResponse;


@Controller
@RequestMapping("/dataController")
public class DataController {
	@Autowired
	private DataService dataService;

	public void setDataService(DataService dataService) {
		this.dataService = dataService;
	}
	
	@ResponseBody
	@RequestMapping("/getCpuData")
	public ExtjsResponse getCpuData(){
		ExtjsResponse extjsResponse = new ExtjsResponse(true);
		System.out.println(extjsResponse);
		extjsResponse.addAttribute("cpu",dataService.getCpuInfo());
		return extjsResponse;
	}
	
	@ResponseBody
	@RequestMapping("/getPhysicalMemory")
	public Memory getPhysicalMemory(){
		return dataService.getPhysicalMemory();
	}
	
	@ResponseBody
	@RequestMapping("/getDiskData")
	public List getDiskData(){
		System.out.println(dataService.getDiskData());
		return dataService.getDiskData();
	}

	@ResponseBody
	@RequestMapping("/queryUserMessage")
	public ExtjsStoreResponse queryUserMessage(String keyword, PagingInfo info) throws SQLException {
        ExtjsStoreResponse response = new  ExtjsStoreResponse();
        Map map = dataService.queryUserMessage(keyword,info);
        response.setRows((List)map.get("result"));
        response.setTotal((Integer) map.get("size"));
        return response;
	}

	@ResponseBody
	@RequestMapping("/getUserRelatedWaterBody")
	public ExtjsStoreResponse getUserRelatedWaterBody(String userId) throws SQLException {
		List<String> list = dataService.getUserRelatedWaterBody(userId);
		ExtjsStoreResponse storeResponse =  new ExtjsStoreResponse();
		storeResponse.setSuccess(true);
		storeResponse.setRows(list);
		return storeResponse;
	}

	@ResponseBody
	@RequestMapping("/saveUserRelatedWaterBody")
	public ExtjsResponse saveUserRelatedWaterBody(@RequestParam String userId,String waterBodyIds) throws Exception {
		dataService.saveUserRelatedWaterBody(userId,waterBodyIds);
		return new ExtjsResponse(true);
	}

	@ResponseBody
	@RequestMapping("/deleteUserRelatedWaterBody")
	public ExtjsResponse deleteUserRelatedWaterBody(@RequestParam String userId) throws Exception {
		dataService.deleteUserRelatedWaterBody(userId);
		return new ExtjsResponse(true);
	}
	
}
