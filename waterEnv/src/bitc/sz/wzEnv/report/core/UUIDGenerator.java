package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.model.FormField;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.UUID;

/**
 * Created by BomWu on 2015/11/25 0025.
 */
@Component("uuid")
public class UUIDGenerator implements ValueGenerator {
    @Override
    public String generatorValue(FormField formField, List<FormField> allFields, List<String> generatorParams) {
        return UUID.randomUUID().toString().replaceAll("-", "");
    }
}
