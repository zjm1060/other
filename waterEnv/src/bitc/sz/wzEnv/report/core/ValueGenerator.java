package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.model.FormField;

import java.util.List;

/**
 * 用于生成内容
 * Created by BomWu on 2015/11/25 0025.
 */
public interface ValueGenerator {
    /**
     * @param formField
     * @param allFields
     * @param generatorParams
     * @return
     */
    String generatorValue(FormField formField, List<FormField> allFields, List<String> generatorParams);
}
