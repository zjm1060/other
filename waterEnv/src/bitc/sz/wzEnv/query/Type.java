package bitc.sz.wzEnv.query;

import java.math.BigDecimal;
import java.sql.Date;
import java.text.ParseException;
import java.util.regex.Pattern;

import bitc.sz.wzEnv.util.DateUtil;
import org.apache.commons.lang.StringUtils;
import org.hibernate.type.BigDecimalType;
import org.hibernate.type.BooleanType;
import org.hibernate.type.DateType;
import org.hibernate.type.DoubleType;
import org.hibernate.type.FloatType;
import org.hibernate.type.IntegerType;
import org.hibernate.type.StringType;


public class Type {

    /**
     * 字符串
     */
    public static final String STRING = "string";
    /**
     * 整型
     */
    public static final String INTEGER = "integer";
    /**
     * 单精度浮点
     */
    public static final String FLOAT = "float";
    /**
     * 双精度浮点
     */
    public static final String DOUBLE = "double";
    /**
     * 日期型，不带时间
     */
    public static final String DATE = "date";
    /**
     * 日期时间型，带时间
     */
    public static final String DATETIME = "datetime";

    public static final String BOOLEAN = "boolean";

    public static final String BIGDECIMAL = "bigdecimal";

    public static final String ENUM = "enum$";

    public static org.hibernate.type.Type getHibernateType(String type) {
        if (STRING.equals(type)) {
            return StringType.INSTANCE;
        } else if (INTEGER.equals(type)) {
            return IntegerType.INSTANCE;
        } else if (DATE.equals(type)) {
            return DateType.INSTANCE;
        } else if (BIGDECIMAL.equals(type)) {
            return BigDecimalType.INSTANCE;
        } else if (DATETIME.equals(type)) {
            return DateType.INSTANCE;
        } else if (BOOLEAN.equals(type)) {
            return BooleanType.INSTANCE;
        } else if (FLOAT.equals(type)) {
            return FloatType.INSTANCE;
        } else if (DOUBLE.equals(type)) {
            return DoubleType.INSTANCE;
        }
        return StringType.INSTANCE;
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public static Object get(String type, String value) {
        if (STRING.equals(type)) {
            return value;
        } else if (INTEGER.equals(type)) {
            return Integer.valueOf(value);
        } else if (BIGDECIMAL.equals(type)) {
            return new BigDecimal(value);
        } else if (BOOLEAN.equals(type)) {
            return Boolean.parseBoolean(value);
        } else if (FLOAT.equals(type)) {
            return Float.valueOf(value);
        } else if (DOUBLE.equals(type)) {
            return Double.valueOf(value);
        } else if (DATE.equals(type)) {
            return Date.valueOf(value);
        } else if (DATETIME.equals(type)) {
            try {
                return DateUtil.parseDateTime(value);
            } catch (ParseException e) {
                throw new RuntimeException(e);
            }
        } else if (type.startsWith(ENUM)) {
            try {
                String enumName = type.substring(5);
                Class c = Class.forName(enumName);
                Enum en = Enum.valueOf(c, value);
                return en;
            } catch (ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
        } else {
            return value;
        }
    }

    @SuppressWarnings({"unchecked", "rawtypes"})
    public static boolean isType(String value, String type) {
        if (StringUtils.isEmpty(type) || StringUtils.isEmpty(value)) {
            return false;
        }
        if (STRING.equals(type)) {
            return true;
        } else if (INTEGER.equals(type)) {
            return Pattern.matches("^-?\\d+$", value);
        } else if (BIGDECIMAL.equals(type)) {
            return Pattern.matches("^-?\\d*\\.?\\d*$", value);
        } else if (BOOLEAN.equals(type)) {
            return "true".equals(value) || "false".equals(value);
        } else if (DATE.equals(type)) {
            return Pattern.matches("^\\d{4}-((0([1-9]{1}))|(1[0|1|2]))-(([0-2]([1-9]{1}))|(3[0|1]))$", value);
        } else if (DATETIME.equals(type)) {
            return Pattern.matches("^\\d{4}-((0([1-9]{1}))|(1[0|1|2]))-(([0-2]([1-9]{1}))|(3[0|1]))\\s((0[0-9])|(1[0-9])|(2[0-3])):([0-5][0-9]):([0-5][0-9])$", value);
        } else if (FLOAT.equals(type)) {
            return Pattern.matches("^-?\\d*\\.?\\d*$", value);
        } else if (DOUBLE.equals(type)) {
            return Pattern.matches("^-?\\d*\\.?\\d*$", value);
        } else if (type.startsWith(ENUM)) {
            String enumName = type.substring(5);
            Class c;
            try {
                c = Class.forName(enumName);
                Enum.valueOf(c, value);
            } catch (Exception e) {
                return false;
            }
            return true;
        }
        return false;
    }

    public static void main(String[] args) {
        System.out.println(isType("2010-03-31 00:18:00", DATETIME));
    }
}
