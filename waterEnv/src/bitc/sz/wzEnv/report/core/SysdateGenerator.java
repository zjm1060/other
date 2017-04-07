package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.model.FormField;
import org.springframework.stereotype.Component;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

/**
 * Created by BomWu on 2015/11/25 0025.
 */
@Component("sysdate")
public class SysdateGenerator implements ValueGenerator {
    private SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    @Override
    public String generatorValue(FormField formField, List<FormField> allFields, List<String> generatorParams) {
        return sdf.format(new Date());
    }
}
