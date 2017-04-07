package bitc.sz.wzEnv.webservice;

import bitc.sz.wzEnv.util.ExtjsResponse;

import java.util.Map;

/**
 * Created by BomWu on 2015/12/8 0008.
 */
public interface WebServiceCommand {
    void execute(Map<String, String> paramMap, ExtjsResponse response);
}
