package test;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.RandomStringUtils;

public class B {
	private String id;
	private List c;

	public B() {
		this.id = RandomStringUtils.random(10);
		c = new ArrayList();
		addC(new C());
		addC(new C());
	}

	private void addC(C c2) {
		c2.setB(this);
		c.add(c2);
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public List getC() {
		return c;
	}

	public void setC(List c) {
		this.c = c;
	}
}