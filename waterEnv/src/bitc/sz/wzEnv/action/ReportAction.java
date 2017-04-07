package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.core.ReportException;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.model.*;
import bitc.sz.wzEnv.report.util.ReportSqlParser;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import bitc.sz.wzEnv.util.FormLoadResponse;
import bitc.sz.wzEnv.util.JsonUtil;
import net.sf.jsqlparser.JSQLParserException;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.shiro.crypto.hash.Hash;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Controller
@RequestMapping("/report")
public class ReportAction {
    @Autowired
    private ReportService reportService;

    @ResponseBody
    @RequestMapping("/dbs")
    public List<DBConnect> getRegisterDB() throws Exception {
        return reportService.getDbConnects();
    }

    @ResponseBody
    @RequestMapping("/meta")
    public ExtjsResponse getGridMeta(@RequestParam String reportToken)
            throws Exception {
        ExtjsResponse ajaxResponse = new ExtjsResponse();
        try {
            Report report = reportService.getReportByName(reportToken);
            ReportMetaData data = new ReportMetaData(report);
            ajaxResponse.setSuccess(true);
            ajaxResponse.addAttribute("meta", data);
        } catch (ReportException e) {
            Logger.getLogger(ReportAction.class).error(e.getMessage());
            ajaxResponse.setSuccess(false);
            ajaxResponse.setMessage(e.getMessage());
            ajaxResponse.setTitle("错误");
        }
        return ajaxResponse;
    }

    @ResponseBody
    @RequestMapping("/load")
    public FormLoadResponse loadReport(@RequestParam String reportId) throws ReportException {
        Report report = reportService.getReport(reportId);
        FormLoadResponse ajaxResponse = new FormLoadResponse(report);
        return ajaxResponse;
    }

    @ResponseBody
    @RequestMapping("/new")
    public ExtjsResponse newReport(Report report, HttpServletRequest request)
            throws Exception {
        ExtjsResponse formSubmitResponse = new ExtjsResponse(true,
                "成功", "处理成功");
        String paramCount = request.getParameter("paramCount");
        List<Parameter> list = new ArrayList<Parameter>();
        int count = paramCount != null ? Integer.valueOf(paramCount) : 0;
        Parameter parameter = null;
        for (int i = 1; i <= count; i++) {
            parameter = new Parameter();
            parameter.setName(report.getName() + "_arg_" + i);
            parameter.setDisplayName(report.getName() + "_arg_" + i);
            parameter.setParameterType(request.getParameter("parameterType" + i) != null && "OUT".equals(request.getParameter("parameterType" + i)) ? ParameterType.OUT : ParameterType.IN);
            parameter.setSort(request.getParameter("sort" + i) != null ? Integer.valueOf(request.getParameter("sort" + i)) : i);
            parameter.setDefaultValue(request.getParameter("defaultValue" + i));
            parameter.setType(request.getParameter("rep_arg_type" + i));
            list.add(parameter);
        }
        if (StringUtils.isEmpty(report.getId())) {
            String reportId = reportService.doInitSQLReport(report, list);
            formSubmitResponse.addAttribute("reportId", reportId);
            formSubmitResponse.addAttribute("reportName", report.getName());
        } else {
            reportService.updateReport(report,list);
        }

        return formSubmitResponse;
    }

    @ResponseBody
    @RequestMapping("/getParametersCount")
    public ExtjsResponse getParametersCount(String sql, String sqlType) {
        List<Parameter> list = new ArrayList<Parameter>();
        ExtjsResponse response = new ExtjsResponse(true);

        if ("PROCEDURE".equals(sqlType)) {
            try {
                int count = 0;
                if (sql != null) {
                    Pattern p0 = Pattern.compile("\\{.*?\\}");// 查找规则公式中大括号以内的字符
                    Matcher m0 = p0.matcher(sql);
                    if(m0.find()) {
                        Pattern p = Pattern.compile("\\(.*?\\)");// 查找规则公式中小括号以内的字符
                        Matcher m = p.matcher(sql);
                        String param = null;
                        while (m.find()) {// 遍历找到的所有小括号
                            param = m.group().replaceAll("\\(", "").replaceAll("\\)", "");// 去掉括号
                            count = param.split(",").length;
                        }
                        response.addAttribute("parameCount", count);
                    }else {
                        response.setSuccess(false);
                        response.addAttribute("parameCount", count);
                        response.setTitle("警告");
                        response.setMessage("请输入正确的存储过程！");
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                response.addAttribute("parameCount", 0);
                response.setSuccess(false);
                response.setTitle("警告");
                response.setMessage("请输入正确的存储过程！");
            }
        } else {
            try {
                ReportSqlParser.sqlParser(list, sql);
                response.addAttribute("parameCount", list.size());
            } catch (Exception e) {
                e.printStackTrace();
                response.addAttribute("parameCount", 0);
                response.setSuccess(false);
                response.setTitle("警告");
                response.setMessage("请输入正确的SQL语句！");
            }
        }


        return response;
    }

    @RequestMapping("/open")
    public String gridReport(HttpServletRequest request, @RequestParam String reportToken)
            throws Exception {
        if (StringUtils.isEmpty(reportToken)) {
            return null;
        }
        request.setAttribute("reportToken", reportToken);
        return "report/gridReport";
    }

    @ResponseBody
    @RequestMapping("/data")
    public ExtjsResponse getGridData(@RequestParam String reportToken,
                                     PagingInfo info, String parameterValues, String reportCondition) throws Exception {
        try {
            if(info.getSort()!= null && info.getSort().indexOf("{")<info.getSort().indexOf(":") && info.getSort().indexOf("}")>info.getSort().indexOf(":")){
                List<HashMap<String,String>> list = JsonUtil.readValue(info.getSort(),ArrayList.class);
                info.setSort(list.get(0).get("property"));
                info.setOrder(list.get(0).get("direction"));
            }
            GridData data = reportService.selectGridData(reportToken, info, parameterValues, reportCondition);
            return data;
        } catch (ReportException e) {
            Logger.getLogger(ReportAction.class).error(e.getMessage());
            return new ExtjsResponse(false,
                    "加载失败", e.getMessage());
        }
    }

    @ResponseBody
    @RequestMapping("/delete")
    public ExtjsResponse deleteReport(@RequestParam String reportId) {
        reportService.deleteReport(reportId);
        return new ExtjsResponse(true);
    }

}
