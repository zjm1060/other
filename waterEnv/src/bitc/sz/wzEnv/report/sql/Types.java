package bitc.sz.wzEnv.report.sql;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.Map;

public class Types {
    private static Map<Integer, String> extjsTypeMapping = new HashMap<Integer, String>();
    private static Map<Integer, String> fieldTypeMapping = new HashMap<Integer, String>();
    public static final String STRING = "string";
    public static final String NUMBER = "number";
    public static final String DATE = "date";
    public static final String DATETIME = "datetime";
    public static final String BOOLEAN = "boolean";

    static {
        extjsTypeMapping.put(null, "auto");
        extjsTypeMapping.put(java.sql.Types.BIGINT, "int");
        extjsTypeMapping.put(java.sql.Types.BOOLEAN, "boolean");
        extjsTypeMapping.put(java.sql.Types.CHAR, "string");
        extjsTypeMapping.put(java.sql.Types.DATE, "date");
        extjsTypeMapping.put(java.sql.Types.DECIMAL, "float");
        extjsTypeMapping.put(java.sql.Types.DOUBLE, "float");
        extjsTypeMapping.put(java.sql.Types.FLOAT, "float");
        extjsTypeMapping.put(java.sql.Types.INTEGER, "int");
        extjsTypeMapping.put(java.sql.Types.LONGVARCHAR, "string");
        extjsTypeMapping.put(java.sql.Types.NUMERIC, "float");
        extjsTypeMapping.put(java.sql.Types.SMALLINT, "int");
        extjsTypeMapping.put(java.sql.Types.TIME, "date");
        extjsTypeMapping.put(java.sql.Types.TIMESTAMP, "date");
        extjsTypeMapping.put(java.sql.Types.TINYINT, "int");
        extjsTypeMapping.put(java.sql.Types.VARCHAR, "string");

        fieldTypeMapping.put(java.sql.Types.BIGINT, NUMBER);
        fieldTypeMapping.put(java.sql.Types.BOOLEAN, BOOLEAN);
        fieldTypeMapping.put(java.sql.Types.CHAR, STRING);
        fieldTypeMapping.put(java.sql.Types.DATE, DATE);
        fieldTypeMapping.put(java.sql.Types.DECIMAL, NUMBER);
        fieldTypeMapping.put(java.sql.Types.DOUBLE, NUMBER);
        fieldTypeMapping.put(java.sql.Types.FLOAT, NUMBER);
        fieldTypeMapping.put(java.sql.Types.INTEGER, NUMBER);
        fieldTypeMapping.put(java.sql.Types.LONGVARCHAR, STRING);
        fieldTypeMapping.put(java.sql.Types.NUMERIC, NUMBER);
        fieldTypeMapping.put(java.sql.Types.SMALLINT, NUMBER);
        fieldTypeMapping.put(java.sql.Types.TIME, DATE);
        fieldTypeMapping.put(java.sql.Types.TIMESTAMP, DATETIME);
        fieldTypeMapping.put(java.sql.Types.TINYINT, NUMBER);
        extjsTypeMapping.put(java.sql.Types.VARCHAR, STRING);
    }

    public static String getExtjsType(Integer javaSqlType) {
        // return extjsTypeMapping.get(javaSqlType);
        return "auto";
    }

    public static String getFieldType(int javaSqlType) {
        String type = fieldTypeMapping.get(javaSqlType);
        return type == null ? STRING : type;
    }

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
    private static final SimpleDateFormat dateTimeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

    public static Object getValue(String valueType, String value) throws Exception {
        if ("".equals(value)) {
            return null;
        }
        if (STRING.equals(valueType)) {
            return value;
        } else if (NUMBER.equals(valueType)) {
            return new BigDecimal(value);
        } else if (DATE.equals(valueType)) {
            return new java.sql.Date(dateFormat.parse(value).getTime());
        } else if (BOOLEAN.equals(valueType)) {
            return Boolean.valueOf(value);
        } else if (DATETIME.equals(valueType)) {
            return new java.sql.Timestamp(dateTimeFormat.parse(value).getTime());
        }
        return value;
    }
}
