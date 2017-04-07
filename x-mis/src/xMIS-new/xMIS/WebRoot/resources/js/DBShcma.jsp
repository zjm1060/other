<%@page language="java" pageEncoding="utf-8"  contentType="text/javascript"%>
<%@page import="java.lang.reflect.Modifier"%>
<%@page import="java.lang.reflect.Field"%>
<%@page import="com.bitc.jk.xmis.util.DBSchema"%>
var DBSchema=function(){};
<%
	Class c = DBSchema.class;
	Class[] cs = c.getClasses();
	for (Class cla : cs) {
		out.print("DBSchema." + cla.getSimpleName()+" = new Object();");
		Field[] fs = cla.getFields();
		for (Field f : fs) {
			if (Modifier.isStatic(f.getModifiers())) {
				out.print("DBSchema." + cla.getSimpleName() + "."
						+ f.getName() + " = '" + f.get(null).toString()
						+ "';");
			}
		}
	}
%>