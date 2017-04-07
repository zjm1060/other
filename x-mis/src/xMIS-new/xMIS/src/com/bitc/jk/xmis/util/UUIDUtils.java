package com.bitc.jk.xmis.util;

import org.apache.commons.lang.StringUtils;

public class UUIDUtils {
	public static String nextCode() {
		return StringUtils.replace(java.util.UUID.randomUUID().toString()
				.toUpperCase(), "-", "");
	}

	public static void main(String[] args) {
		System.out.println(nextCode());
	}
}
