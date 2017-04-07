package test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringEscapeUtils;
import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.util.SQLScript;

public class A {
	private List list;

	public A() {
		list = new ArrayList();
		list.add(new B());
	}

	public List getList() {
		return list;
	}

	public void setList(List list) {
		this.list = list;
	}

}