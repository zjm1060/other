package bitc.sz.wzEnv.model;

import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/5/12.
 */
public class WebServiceRiverWaterFounctionCondition {
    private String message;
    private int responseSts;
    private int totalCount;
    private List<Map<Object,Object>> nodeList;
    private List<Map<String,String>> propFields;

    public List<Map<Object, Object>> getNodeList() {
        return nodeList;
    }

    public void setNodeList(List<Map<Object, Object>> nodeList) {
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
