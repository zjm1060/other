package bitc.sz.wzEnv.action;

import bitc.sz.wzEnv.report.core.DeleteCommand;
import bitc.sz.wzEnv.report.core.FormException;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.model.CombField;
import bitc.sz.wzEnv.report.model.Field;
import bitc.sz.wzEnv.report.model.Form;
import bitc.sz.wzEnv.report.model.FormField;
import bitc.sz.wzEnv.util.ExtjsResponse;
import bitc.sz.wzEnv.util.ExtjsStoreResponse;
import bitc.sz.wzEnv.util.FormLoadResponse;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by BomWu on 2015/11/16 0016.
 */
@Controller
@RequestMapping("/form")
public class FormAction {
    @Autowired
    private ReportService reportService;

    @RequestMapping("/new")
    public void newForm(Form form, @RequestParam(required = true) String tableName, @RequestParam(required = true) String rowKey) throws Exception {
        reportService.doInitForm(form, tableName, rowKey);
    }

    @ResponseBody
    @RequestMapping("/meta")
    public ExtjsResponse getFormMeta(@RequestParam(required = true) String formToken)
            throws Exception {
        ExtjsResponse ajaxResponse = new ExtjsResponse();
        try {
            Form form = reportService.getFormByName(formToken);
            ajaxResponse.setSuccess(true);
            ajaxResponse.addAttribute("meta", form);
        } catch (FormException e) {
            Logger.getLogger(ReportAction.class).error(e.getMessage());
            ajaxResponse.setSuccess(false);
            ajaxResponse.setMessage(e.getMessage());
            ajaxResponse.setTitle("获取信息失败");
        }
        return ajaxResponse;
    }

    @ResponseBody
    @RequestMapping("/load")
    public ExtjsResponse getGridMeta(@RequestParam String formId) {
        return new FormLoadResponse(reportService.getForm(formId));

    }

    @ResponseBody
    @RequestMapping("/save")
    public ExtjsResponse save(@RequestParam String formToken, HttpServletRequest request) throws Exception {
        ExtjsResponse ajaxResponse = new ExtjsResponse();
        Form form = reportService.getFormByName(formToken);
        List<FormField> formFields = new ArrayList<FormField>();
        for (int i = 0; i < form.getFields().size(); i++) {
            Field f = form.getFields().get(i);
            String value = request.getParameter(f.getName());
            String rowKeyValue = request.getParameter(f.getName() + "_rowKeyValue");
            String submitValue = request.getParameter(f.getName() + "_submitValue");
            if ("true".equals(submitValue)) {
                FormField formfield = new FormField(f.getTableName(), f.getName(), value, f.getRowKey(), rowKeyValue, f.getSqlType());
                formfield.setField(f);
                formFields.add(formfield);
            }
        }
        reportService.saveOrUpdate(form, formFields);
        ajaxResponse.setSuccess(true);
        return ajaxResponse;
    }

    @ResponseBody
    @RequestMapping("/combo")
    public ExtjsStoreResponse getCombos(CombField field) throws Exception {
        ExtjsStoreResponse store = new ExtjsStoreResponse();
        List<CombField> list = reportService.getCombos(field);
        store.setRows(list);
        store.setTotal(list.size());
        return store;
    }

    @ResponseBody
    @RequestMapping("/delete")
    public ExtjsResponse delete(@RequestBody DeleteCommand deleteCommand) throws Exception {
        reportService.deleteRows(deleteCommand);
        return new ExtjsResponse(true);
    }
//    @ResponseBody
//    @RequestMapping("/upload")
//    public ExtjsResponse upload(@RequestParam MultipartFile[] myfiles) throws Exception {
//        MultipartFile[] multipartFile = myfiles;
//        File file = new File("d:\\aa.doc");
//        if(!file.exists()){
//            file.createNewFile();
//        }
//        OutputStream fos = new FileOutputStream(file);
//        fos.write(multipartFile[1].getBytes());
//        fos.close();
//        return new ExtjsResponse(true);
//    }
//        @RequestMapping("download")
//        public ResponseEntity<byte[]> download() throws IOException {
//            String path="d:\\aa.doc";
//            File file=new File(path);
//            HttpHeaders headers = new HttpHeaders();
//            String fileName=new String("你好.doc".getBytes("UTF-8"),"iso-8859-1");//为了解决中文名称乱码问题
//            headers.setContentDispositionFormData("attachment", fileName);
//            headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
//            return new ResponseEntity<byte[]>(FileUtils.readFileToByteArray(file),
//                    headers, HttpStatus.OK);
//        }
}
