package com.bitc.jk.xmis.web.response;

import java.io.Serializable;
import java.sql.Date;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;

import net.sf.json.JSONSerializer;
import net.sf.json.JsonConfig;
import net.sf.json.processors.JsonValueProcessor;
import net.sf.json.util.CycleDetectionStrategy;

/**
 * Json响应，抽象类
 * 
 * @author Bom Wu
 * 
 */
public abstract class JsonResponse implements Serializable {

	private static final long serialVersionUID = -1474643293515605639L;

	private JsonConfig config = new JsonConfig();

	public JsonResponse() {
		config.registerJsonValueProcessor(Date.class, new JsonValueProcessor() {
			public Object processObjectValue(String key, Object value,
					JsonConfig jsonConfig) {
				if (value == null) {
					return null;
				}
				Date date = (Date) value;
				return date.toString();
			}

			public Object processArrayValue(Object arg0, JsonConfig arg1) {
				return null;
			}
		});
		config.registerJsonValueProcessor(java.util.Date.class,
				new JsonValueProcessor() {
					private SimpleDateFormat sdf = new SimpleDateFormat(
							"yyyy-MM-dd HH:mm:ss");

					public Object processObjectValue(String key, Object value,
							JsonConfig jsonConfig) {
						if (value == null) {
							return null;
						}
						java.util.Date date = (java.util.Date) value;
						return sdf.format(date);
					}

					public Object processArrayValue(Object arg0, JsonConfig arg1) {
						return null;
					}
				});
		config.registerJsonValueProcessor(Timestamp.class,
				new JsonValueProcessor() {

					public Object processArrayValue(Object obj,
							JsonConfig jsonconfig) {
						return null;
					}

					public Object processObjectValue(String key, Object value,
							JsonConfig jsonconfig) {
						if (value == null) {
							return null;
						}
						Timestamp s = (Timestamp) value;
						return s.getTime();
					}
				});
		config.setExcludes(new String[] { "hibernateLazyInitializer",
				"handler", "fieldHandler" });
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		config.setIgnoreTransientFields(true);
	}

	public String toJsonString() {
		return toJsonString(config);
	}

	public String toJsonString(JsonConfig config) {
		return JSONSerializer.toJSON(this, config).toString();
	}

	@Override
	public String toString() {
		return toJsonString();
	}

}
