package bitc.sz.wzEnv.model;

import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/5/11.
 */
public class WebServiceXZQCondition {
    private String message;
    private int responseSts;
    private int totalCount;
    private List<Map<String,Object>> nodeList;
    private List<Map<String,String>> propFields;

    public List<Map<String, Object>> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<Map<String, Object>> nodeList) {
        this.nodeList = nodeList;
    }

    public List<Map<String, String>> getPropFields() {
        return propFields;
    }

    public void setPropFields(List<Map<String, String>> propFields) {
        this.propFields = propFields;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public int getResponseSts() {
        return responseSts;
    }

    public void setResponseSts(int responseSts) {
        this.responseSts = responseSts;
    }

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }

}
