package com.bitc.jk.xmis.util;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;

public class DBSchemaOutput {

	public static void main(String[] args) {
		System.out.println(SQLScript.getContractListByType);
	}

	@SuppressWarnings("rawtypes")
	public static void dbshemaOutput() throws IllegalArgumentException,
			IllegalAccessException {
		Class c = DBSchema.class;
		Class[] cs = c.getClasses();
		System.out.println("var DBSchema=function(){};");
		for (Class cla : cs) {
			Field[] fs = cla.getFields();
			{
				for (Field f : fs) {
					if (Modifier.isStatic(f.getModifiers())) {
						System.out.println("DBSchema." + cla.getSimpleName()
								+ "." + f.getName() + " = '"
								+ f.get(null).toString() + "';");
					}
				}
			}
		}
	}
}
