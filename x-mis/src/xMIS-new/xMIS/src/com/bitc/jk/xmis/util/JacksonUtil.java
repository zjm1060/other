package com.bitc.jk.xmis.util;

import java.io.IOException;
import java.io.StringWriter;

import org.codehaus.jackson.JsonGenerationException;
import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.type.TypeReference;

public class JacksonUtil {
	private static ObjectMapper objectMapper = new ObjectMapper();

	public static String getJsonString(Object object)
			throws JsonGenerationException, JsonMappingException, IOException {
		StringWriter writer = new StringWriter();
		writeValue(object, writer);
		return writer.toString();
	}

	public static void writeValue(Object object, StringWriter writer)
			throws JsonGenerationException, JsonMappingException, IOException {
		objectMapper.writeValue(writer, object);
	}

	public static <T> T readValue(String content, Class<T> clazz)
			throws JsonParseException, JsonMappingException, IOException {
		return objectMapper.readValue(content, clazz);
	}

	public static <T> T readValue(String content, TypeReference<T> typeRef)
			throws JsonParseException, JsonMappingException, IOException {
		return objectMapper.readValue(content, typeRef);
	}
}
