package bitc.sz.wzEnv.webservice;

import bitc.sz.wzEnv.sys.spring.SpringContextHolder;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.JsonUtil;
import org.codehaus.jackson.type.TypeReference;

import javax.jws.WebService;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by BomWu on 2015/12/8 0008.
 */
@WebService
public class QueryServiceT {

   public String query(String command, String paramJSON) {
        WebServiceCommand serviceCommand = (WebServiceCommand) SpringContextHolder.getBean("command");
        ExtjsResponse response = new ExtjsResponse();
        if (serviceCommand == null) {
            response.setSuccess(false);
            response.setMessage("没有找到该服务接口：" + command);
            return JsonUtil.getJsonString(response);
        }
        Map<String, String> paramMap;
        try {
            if (paramJSON == null) {
                paramMap = JsonUtil.readValue(paramJSON, new TypeReference<Map<String, String>>() {
                });
            } else {
                paramMap = new HashMap<String, String>();
            }
        } catch (Exception e) {
            response.setSuccess(false);
            response.setMessage("解析参数出错。" + command);
            return JsonUtil.getJsonString(response);
        }
        serviceCommand.execute(paramMap, response);
        return JsonUtil.getJsonString(response);
    }
}
