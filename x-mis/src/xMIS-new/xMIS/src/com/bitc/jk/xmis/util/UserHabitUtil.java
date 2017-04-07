package com.bitc.jk.xmis.util;

import java.util.List;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import com.bitc.jk.xmis.model.sys.UserHabit;

public class UserHabitUtil {

	public static UserHabit buildUserHabit(String habitString,
			Integer employeeID) {
		UserHabit habit = new UserHabit();
		habit.setEmployeeID(employeeID);
		Document document = DocumentHelper.createDocument();
		Element root = document.addElement("habits");
		Element habitEl = root.addElement("habit");
		habitEl.addText(habitString);
		habit.setHabit(document.asXML());
		return habit;
	}

	public static String getHabitString(UserHabit habit)
			throws DocumentException {
		Document document = DocumentHelper.parseText(habit.getHabit());
		List list = document.selectNodes("/habits/habit");
		Element e = (Element) list.get(0);
		return e.getText();
	}

}