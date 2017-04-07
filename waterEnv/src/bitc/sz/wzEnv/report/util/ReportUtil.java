package bitc.sz.wzEnv.report.util;

import bitc.sz.wzEnv.report.core.ValueGenerator;
import bitc.sz.wzEnv.report.model.Field;
import bitc.sz.wzEnv.report.model.FormField;
import bitc.sz.wzEnv.sys.spring.SpringContextHolder;
import org.apache.commons.lang.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Administrator on 2015/11/23.
 */
public class ReportUtil {

    public static FormField findFormField(List<FormField> formFieldList, String tableName, String name) {
        for (FormField formField : formFieldList) {
            if (formField.getTableName().equalsIgnoreCase(tableName) && formField.getName().equalsIgnoreCase(name)) {
                return formField;
            }
        }
        return null;
    }

    public static String generateInsertValue(FormField formField, List<FormField> allFields) {
        String valueGenerator = formField.getField().getInsertValueGenerator();
        return generateValue(formField, allFields, valueGenerator);
    }

    public static String generateUpdateValue(FormField formField, List<FormField> allFields) {
        String valueGenerator = formField.getField().getUpdateValueGenerator();
        return generateValue(formField, allFields, valueGenerator);

    }

    private static String generateValue(FormField formField, List<FormField> allFields, String valueGenerator) {
        if (StringUtils.isEmpty(valueGenerator)) {
            return null;
        }
        String[] strs = StringUtils.split(valueGenerator, "?");
        ValueGenerator generator = (ValueGenerator) SpringContextHolder.getBean(strs[0]);
        if (generator == null) {
            return null;
        }
        List<String> params = new ArrayList<String>();
        if (strs.length > 1) {
            for (String param : StringUtils.split(strs[1], "&")) {
                params.add(param);
            }
        }
        return generator.generatorValue(formField, allFields, params);
    }
}
