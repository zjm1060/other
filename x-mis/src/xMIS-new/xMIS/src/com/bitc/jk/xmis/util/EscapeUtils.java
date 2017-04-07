package com.bitc.jk.xmis.util;

import java.io.IOException;
import java.io.StringWriter;
import java.io.Writer;
import java.util.Locale;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.UnhandledException;

public class EscapeUtils {

	public static String escapeDoubleQuotationMarks(String str) {
		return StringUtils.replace(StringUtils.replace(str, "\\", "\\\\"),
				"\"", "\\\"");
	}

	public static String escape(String str) {
		if (str == null) {
			return null;
		}
		try {
			StringWriter writer = new StringWriter(str.length() * 2);
			escape(writer, str);
			return writer.toString();
		} catch (IOException ioe) {
			throw new UnhandledException(ioe);
		}
	}

	public static void escape(Writer out, String str) throws IOException {
		if (out == null) {
			throw new IllegalArgumentException("The Writer must not be null");
		}
		if (str == null) {
			return;
		}
		int sz;
		sz = str.length();
		for (int i = 0; i < sz; i++) {
			char ch = str.charAt(i);
			if (ch > 0xfff) {
				out.write("\\u" + hex(ch));
			} else if (ch > 0xff) {
				out.write("\\u0" + hex(ch));
			} else if (ch > 0x7f) {
				out.write("\\u00" + hex(ch));
			} else if (ch < 32) {
				switch (ch) {
				case '\b':
					out.write('\\');
					out.write('b');
					break;
				case '\n':
					out.write('\\');
					out.write('n');
					break;
				case '\t':
					out.write('\\');
					out.write('t');
					break;
				case '\f':
					out.write('\\');
					out.write('f');
					break;
				case '\r':
					out.write('\\');
					out.write('r');
					break;
				default:
					if (ch > 0xf) {
						out.write("\\u00" + hex(ch));
					} else {
						out.write("\\u000" + hex(ch));
					}
					break;
				}
			} else {
				out.write(ch);
			}
		}
	}

	private static String hex(char ch) {
		return Integer.toHexString(ch).toUpperCase(Locale.ENGLISH);
	}

}
