package com.bitc.jk.xmis.export.web;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;

class RequestWrapper extends HttpServletRequestWrapper {
	private Map<String, Object> excepts;
	private String requestUrl;

	public RequestWrapper(HttpServletRequest request, String requestUrl) {
		super(request);
		excepts = new HashMap<String, Object>();
		this.requestUrl = requestUrl;
	}

	public void addExcept(List<String[]> keyValues) {
		for (String[] keyvalue : keyValues) {
			addExcept(keyvalue[0], keyvalue[1]);
		}
	}

	public void addExcept(String name, String value) {
		if (excepts.containsKey(name)) {
			Object oldvalue = excepts.get(name);
			if (oldvalue instanceof String) {
				excepts.put(name, new String[] { (String) oldvalue, value });
			} else {
				String[] v = (String[]) oldvalue;
				excepts.put(name, ArrayUtils.add(v, value));
			}
		} else {
			excepts.put(name, value);
		}
	}

	@Override
	public String getParameter(String name) {
		if (excepts.containsKey(name)) {
			Object value = excepts.get(name);
			if (value instanceof String) {
				return (String) value;
			} else {
				return null;
			}
		}
		return super.getParameter(name);
	}

	@Override
	public String[] getParameterValues(String name) {
		if (excepts.containsKey(name)) {
			Object value = excepts.get(name);
			if (value instanceof String[]) {
				return (String[]) value;
			} else {
				return null;
			}
		}
		return super.getParameterValues(name);
	}

	@Override
	public String getRequestURI() {
		return StringUtils.split(this.requestUrl, '?')[0];
	}

	@Override
	public StringBuffer getRequestURL() {
		return new StringBuffer(this.requestUrl);
	}

	@Override
	public String getServletPath() {
		return getRequestURI().substring(getContextPath().length());
	}

	@Override
	public String getQueryString() {
		int index = this.requestUrl.indexOf('?');
		return index == -1 ? null : this.requestUrl.substring(index + 1);
	}
}
