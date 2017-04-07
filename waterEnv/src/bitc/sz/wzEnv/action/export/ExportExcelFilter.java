package bitc.sz.wzEnv.action.export;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.util.JsonUtil;
import bitc.sz.wzEnv.util.WebUtil;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import javax.script.Invocable;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by BomWu on 2016/2/3 0003.
 */
public class ExportExcelFilter implements Filter {
    public void destroy() {
    }

    public void doFilter(ServletRequest req, ServletResponse resp, FilterChain chain) throws ServletException, IOException {
        if ("true".equals(req.getParameter("itsaexporttoexcelrequest"))) {
            RequestWrapper request = new RequestWrapper((HttpServletRequest) req);
            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            ResponseWrapper response = new ResponseWrapper((HttpServletResponse) resp, byteArrayOutputStream);
            chain.doFilter(request, response);
            String responseText = new String(byteArrayOutputStream.toByteArray(), "UTF-8");
            export((HttpServletRequest) req, (HttpServletResponse) resp, responseText);
        } else if("true".equals(req.getParameter("notreportexporttoexcelrequest"))){
            RequestWrapper request = new RequestWrapper((HttpServletRequest) req);
            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            ResponseWrapper response = new ResponseWrapper((HttpServletResponse) resp, byteArrayOutputStream);
            chain.doFilter(request, response);
            String responseText = new String(byteArrayOutputStream.toByteArray(), "UTF-8");
            notReportExport((HttpServletRequest) req, (HttpServletResponse) resp, responseText);
        }else {
            chain.doFilter(req, resp);
        }
    }

    private void notReportExport(HttpServletRequest request, HttpServletResponse response, String responseText) throws IOException {
        String cms = request.getParameter("exp_cms");
        List<Map<String,String>> columns = JsonUtil.readJson(cms, ArrayList.class,Map.class);
        Map<String,Object> resultMap = JsonUtil.readValue(responseText,Map.class);
        List<Map<String,String>> rows = (List<Map<String, String>>) resultMap.get("rows");
        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet();
        HSSFRow head = sheet.createRow(0);
        for (int i = 0; i < columns.size(); i++) {
            HSSFCell cell = head.createCell(i);
            cell.setCellValue((columns.get(i).get("dataIndex")));
        }
        for (int i = 0; i < rows.size(); i++) {
            HSSFRow row = sheet.createRow(i + 1);
            Map<String,String> jsonRow = rows.get(i);
            for (int j = 0; j < columns.size(); j++) {
                HSSFCell cell = row.createCell(j);
                String mapping = columns.get(j).get("dataIndex");
                cell.setCellValue(jsonRow.get(mapping));
            }
        }
        // 写入excel
        String name = request.getParameter("exp_name");
        String fileName = name+ "_数据导出.xls";
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        response.setContentType("application/vnd.ms-excel");
        String filename = new String(fileName.getBytes("UTF-8"), "ISO8859-1");
        response.setHeader("Content-Disposition", "attachment;filename=" + filename);
        OutputStream out = response.getOutputStream();

        workbook.write(out);
        workbook.close();


    }

    private void export(HttpServletRequest request, HttpServletResponse response, String responseText) throws IOException {
        JSONObject result = JSON.parseObject(responseText);
        if (result.getBoolean("success") != true) {
            response.getWriter().write(responseText);
            return;
        }
        PagingInfo pageInfo = WebUtil.generatePageInfo(request);
        String cms = request.getParameter("exp_cms");
        JSONArray columns = JSON.parseArray(cms);
        JSONArray rowsArray = result.getJSONArray("rows");

        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet();
        HSSFRow head = sheet.createRow(0);
        for (int i = 0; i < columns.size(); i++) {
            HSSFCell cell = head.createCell(i);
            cell.setCellValue(((JSONObject) columns.get(i)).getString("header"));
        }
        for (int i = 0; i < rowsArray.size(); i++) {
            HSSFRow row = sheet.createRow(i + 1);
            JSONObject jsonRow = ((JSONObject) rowsArray.get(i));
            for (int j = 0; j < columns.size(); j++) {
                HSSFCell cell = row.createCell(j);
                JSONObject column = (JSONObject) columns.get(j);
                String mapping = column.getString("mapping");
                String formatterScript = column.getString("exportFormatter");
                if (formatterScript == null) {
                    cell.setCellValue(jsonRow.getString(mapping));
                } else {
                    Formatter formatter = getFormatter(formatterScript);
                    if (formatter != null) {
                        cell.setCellValue(formatter.getFormatValue(jsonRow.getString(mapping), jsonRow, i, pageInfo));
                    }
                }
            }
        }
        // 写入excel
        String name = request.getParameter("exp_name");
        String fileName = name+ "_数据导出.xls";
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        response.setContentType("application/vnd.ms-excel");
        String filename = new String(fileName.getBytes("UTF-8"), "ISO8859-1");
        response.setHeader("Content-Disposition", "attachment;filename=" + filename);
        OutputStream out = response.getOutputStream();

        workbook.write(out);
        workbook.close();
    }

    private Formatter getFormatter(String formatterScript) {
        try {
            if (formatterScript != null) {
                String script = formatterScript.replace("function",
                        "function getFormatValue");
                ScriptEngine engine = new ScriptEngineManager()  //java调用js代码
                        .getEngineByName("javascript");
                engine.eval(script); //解析脚本
                Invocable jsInvoke = (Invocable) engine;
                //返回一个接口的实现，该接口使用解释器中编译的函数。该接口的方法是使用 invokeFunction 方法实现的。
                //参数 要返回的接口的 Class 对象
                //返回的是所请求接口的实例
                return jsInvoke.getInterface(Formatter.class);
            }
        } catch (Exception e) {
            return null;
        }
        return null;
    }

    public void init(FilterConfig config) throws ServletException {

    }

}
