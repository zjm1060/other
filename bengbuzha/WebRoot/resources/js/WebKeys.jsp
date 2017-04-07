<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page language="java" pageEncoding="utf-8" contentType="text/javascript"%>
<%@page import="java.lang.reflect.Modifier"%>
<%@page import="java.lang.reflect.Field"%>
<%
	Class c = WebKeys.class;
	Field[] fs = c.getFields();
	for (Field f : fs) {
		if (Modifier.isStatic(f.getModifiers())) {
			if (f.getName().indexOf("Color") >= 0) {
				out.print("var " + f.getName() + " = '"
						+ f.get(null).toString() + "';\n");
			}else if (f.getName().indexOf("Upload_Application") >= 0) {
				out.print("var " + f.getName() + " = '"
						+ f.get(null).toString() + "';\n");
			}else if (f.getName().indexOf("Type_") >= 0) {
				out.print("var " + f.getName() + " = '"
						+ f.get(null).toString() + "';\n");
			}else if (f.getName().indexOf("Tip_") >= 0) {
				out.print("var " + f.getName() + " = '"
						+ f.get(null).toString() + "';\n");
			}
		}
	}
	
%>