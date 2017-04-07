package test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.NoSuchAlgorithmException;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.util.CryptUtil;
import com.bitc.jk.xmis.util.SQLScript;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class MapObject {
	private Map<String, String> map2 = new HashMap<String, String>();

	public MapObject() {
		map2.put("key1", "value1\r\n'222'\\中国");
	}

	public Map<String, String> getMap() {
		return map2;
	}

	public void setMap(Map<String, String> map) {
		this.map2 = map;
	}

	public static void main(String[] args) throws IOException,
			NoSuchAlgorithmException {
		String relatedNo = StringUtils.substringBefore(
				"JK-2011-JS-007/Q45", "/Q");
		String sql = StringUtils.replace(SQLScript.getContractInfoByProjectNo,
				"?", "'%" + relatedNo + "'");
		System.out.println(sql);

	}
}