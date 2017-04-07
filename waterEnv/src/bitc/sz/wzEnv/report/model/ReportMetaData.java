package bitc.sz.wzEnv.report.model;

import com.fasterxml.jackson.annotation.JsonIgnore;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReportMetaData {

    @JsonIgnore
    private Report report;

    public ReportMetaData(Report report) {
        this.report = report;
    }

    public List<Map<String, Object>> getCm() {
        List<Map<String, Object>> cm = new ArrayList<Map<String, Object>>();
        Map<String, Object> model;
        for (int i = 0; i < report.getColumns().size(); i++) {
            Column column = report.getColumns().get(i);
            model = new HashMap<String, Object>();
            Map<String, Object> extraConfig = column.getExtraConfig();
            if (extraConfig != null) {
                model.putAll(extraConfig);
            }
//            model.put("id", column.getId());
            model.put("header", column.getDname());
            model.put("dataIndex", column.getName());
            model.put("hidden", column.isHidden());
            cm.add(model);
        }
        return cm;
    }

    public String getTitle() {
        return report.getTitle();
    }

    public ReaderMetaData getReaderMetaData() {
        ReaderMetaData readerMetaData = new ReaderMetaData();
        List<ReaderField> fields = new ArrayList<ReaderField>(report.getColumns().size());
        for (int i = 0; i < report.getColumns().size(); i++) {
            Column column = report.getColumns().get(i);
            ReaderField field = new ReaderField();
            field.setMapping(column.getName());
            field.setName(column.getName());
            field.setType(column.getType());
            fields.add(field);
        }
        readerMetaData.setFields(fields);
        return readerMetaData;
    }

    public String getPlugins() {
        return this.report.getPlugins();
    }

    public boolean isPaging() {
        return this.report.getIsPaging();
    }

    public String getId() {
        return this.report.getId();
    }
}
