package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.model.FormField;
import bitc.sz.wzEnv.report.util.ReportUtil;
import org.springframework.stereotype.Component;

import java.util.List;

/**
 * Created by BomWu on 2015/11/25 0025.
 */
@Component("association")
public class AssociationGenerator implements ValueGenerator {
    @Override
    public String generatorValue(FormField formField, List<FormField> allFields, List<String> generatorParams) {
        if (generatorParams.size() != 2) {
            return null;
        }
        String tableName = generatorParams.get(0);
        String fieldName = generatorParams.get(1);
        FormField field = ReportUtil.findFormField(allFields, tableName, fieldName);
        if (field == null) {
            return null;
        }
        return field.getValue();
    }
}
