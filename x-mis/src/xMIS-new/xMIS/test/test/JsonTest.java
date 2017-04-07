package test;

import java.util.HashMap;
import java.util.Map;

import com.bitc.jk.xmis.util.WebUtil;

import net.sf.json.JSONObject;

public class JsonTest {

	private Map<String, MapObject> map = new HashMap<String, MapObject>();

	private String attr1 = "a";

	public JsonTest() {
		map.put("map1", new MapObject());
	}

	public static void main(String[] args) {

	}

	public String getAttr1() {
		return attr1;
	}

	public void setAttr1(String attr1) {
		this.attr1 = attr1;
	}

	public Map<String, MapObject> getMap() {
		return map;
	}

	public void setMap(Map<String, MapObject> map) {
		this.map = map;
	}
}
