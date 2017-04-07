package com.bitc.jk.xmis.util;

import java.sql.Date;
import java.sql.Time;
import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;

import org.apache.commons.lang.StringUtils;

import net.sf.json.JSON;
import net.sf.json.JSONNull;
import net.sf.json.JSONObject;
import net.sf.json.JSONSerializer;
import net.sf.json.JsonConfig;
import net.sf.json.processors.JsonValueProcessor;
import net.sf.json.util.CycleDetectionStrategy;

public class JSONUtils {
	public static JSON toJSON(Object object, boolean ignoreTransientFields) {
		JsonConfig config = new JsonConfig();
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
		config.registerJsonValueProcessor(Time.class, new JsonValueProcessor() {

			@Override
			public Object processObjectValue(String key, Object value,
					JsonConfig jsonConfig) {
				Time time = (Time) value;
				return time.toString();
			}

			@Override
			public Object processArrayValue(Object arg0, JsonConfig arg1) {
				return null;
			}
		});
		config.registerJsonValueProcessor(Timestamp.class,
				new JsonValueProcessor() {

					@Override
					public Object processObjectValue(String key, Object value,
							JsonConfig jsonConfig) {
						Timestamp timestamp = (Timestamp)value;
						String time= timestamp.toString().substring(0, 10);
						return time;
					}

					@Override
					public Object processArrayValue(Object arg0, JsonConfig arg1) {
						return null;
					}
				});

		config.setExcludes(new String[] { "hibernateLazyInitializer",
				"handler", "fieldHandler" });
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		config.setIgnoreTransientFields(ignoreTransientFields);
		return JSONSerializer.toJSON(object, config);
	}

	public static JSON toJSON(Object object, JsonConfig config) {
		if (config == null) {
			config = new JsonConfig();
		}
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
		String[] exs = config.getExcludes();
		exs = org.springframework.util.StringUtils.concatenateStringArrays(exs,
				new String[] { "hibernateLazyInitializer", "handler",
						"fieldHandler" });
		config.setExcludes(exs);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONSerializer.toJSON(object, config);
	}

	public static Object get(JSONObject object, String key) {
		if (key == null) {
			return null;
		}
		int index = key.indexOf('.');
		if (index > -1) {
			String newKey = key.substring(0, index);
			if (object.containsKey(newKey)) {
				JSONObject obj = object.getJSONObject(newKey);
				return get(obj, key.substring(index + 1));
			} else {
				return null;
			}
		}
		return object.containsKey(key) ? object.get(key) : null;
	}

	public static JSONObject getJSONObject(JSONObject object, String key) {
		if (key == null) {
			return null;
		}
		int index = key.indexOf('.');
		if (index > -1) {
			JSONObject obj = object.getJSONObject(key.substring(0, index));
			return getJSONObject(obj, key.substring(index + 1));
		}
		return object.getJSONObject(key);
	}

	/**
	 * 替代JSONObject.getString(key)方法，更好的处理null值
	 * 
	 * @param object
	 *            JSONObject
	 * @param key
	 *            the key
	 * @return String value
	 */
	public static String getString(JSONObject object, String key) {
		if (key == null) {
			return null;
		}
		int index = key.indexOf('.');
		if (index > -1) {
			JSONObject obj = object.getJSONObject(key.substring(0, index));
			return getString(obj, key.substring(index + 1));
		}
		Object value = object.get(key);
		if (value == null) {
			return null;
		}
		if (value instanceof JSONNull) {
			return null;
		}
		return value.toString();
	}

	public static Integer getInteger(JSONObject object, String key) {
		if (key == null) {
			return null;
		}
		int index = key.indexOf('.');
		if (index > -1) {
			JSONObject obj = object.getJSONObject(key.substring(0, index));
			return getInteger(obj, key.substring(index + 1));
		}
		Object value = object.get(key);
		if (value == null) {
			return null;
		}
		if (value instanceof JSONNull) {
			return null;
		}
		return (Integer) value;
	}

	public static java.util.Date getDate(JSONObject object, String key)
			throws ParseException {
		if (key == null) {
			return null;
		}
		int index = key.indexOf('.');
		if (index > -1) {
			JSONObject obj = object.getJSONObject(key.substring(0, index));
			return getDate(obj, key.substring(index + 1));
		}
		Object value = object.get(key);
		if (value == null) {
			return null;
		}
		if (value instanceof JSONNull) {
			return null;
		}
		String stringValue = object.getString(key);
		if (StringUtils.isEmpty(stringValue)) {
			return null;
		}
		return DateUtil.parseDateTime(stringValue);
	}

}
