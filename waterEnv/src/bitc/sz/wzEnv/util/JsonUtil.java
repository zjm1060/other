package bitc.sz.wzEnv.util;

import java.io.IOException;
import java.io.StringWriter;

import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.type.JavaType;
import org.codehaus.jackson.type.TypeReference;

public class JsonUtil {
	private static ObjectMapper objectMapper = new ObjectMapper();

	public static String getJsonString(Object object) {
		StringWriter writer = new StringWriter();
		writeValue(object, writer);
		return writer.toString();
	}

	public static void writeValue(Object object, StringWriter writer) {
		try {
			objectMapper.writeValue(writer, object);
		} catch (Exception e) {
			throw new RuntimeException("JSON转换异常。", e);
		}
	}

	/**
	 * 
	 * @param <T>
	 * @param content
	 * @param clazz
	 * @return
	 */

	public static <T> T readValue(String content, Class<T> clazz) {
		try {
			return objectMapper.readValue(content, clazz);
		} catch (Exception e) {
			throw new RuntimeException("JSON转换异常。", e);
		}
	}

	public static <T> T readValue(String content, TypeReference<T> typeReference) {
		try {
			return (T)objectMapper.readValue(content, typeReference);
		} catch (Exception e) {
			throw new RuntimeException("JSON转换异常。", e);
		}
	}

	public static <T> T readJson(String jsonStr, Class<?> collectionClass, Class<?>... elementClasses) {

		JavaType javaType = objectMapper.getTypeFactory().constructParametricType(collectionClass, elementClasses);

		try {
			return (T)objectMapper.readValue(jsonStr, javaType);
		} catch (IOException e) {
			throw new RuntimeException("JSON转换异常。", e);
		}

	}
}
