package bitc.sz.wzEnv.model;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/3/9.
 */
public class WebServiceObjDetailInfo {
    private String message;
    private int responseSts;
    private List<Map<String,Object>> surveyData;

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

    public List<Map<String, Object>> getSurveyData() {
        return surveyData;
    }

    public void setSurveyData(List<Map<String, Object>> surveyData) {
        this.surveyData = surveyData;
    }

}
