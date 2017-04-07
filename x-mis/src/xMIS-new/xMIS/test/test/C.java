package test;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.lang.RandomStringUtils;
import org.apache.commons.lang.StringUtils;

public class C {
	private String id;
	private transient B b;

	public C() {
		this.id = RandomStringUtils.random(10);
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public B getB() {
		return b;
	}

	public void setB(B b) {
		this.b = b;
	}

	public static void main(String[] args) throws IOException {
		// URL url = new
		// URL("http://www.zjjyk.com/admin/default.asp?action=login");
		HttpClient client = new HttpClient();
		PostMethod postMethod = new PostMethod(
				"http://www.zjjyk.com/admin/default.asp?action=login");
		postMethod.addParameter("password", "123456");
		postMethod.addParameter("Submit", "Login ...");
		int status = client.executeMethod(postMethod);
		if (status == HttpStatus.SC_OK) {

			System.out.println("状态码：" + postMethod.getStatusCode()
					+ ",HttpStatus.SC_OK=" + HttpStatus.SC_OK);
			System.out.println(postMethod.getStatusLine()); // 打印服务器返回的状态
			// String response =new
			// String(onePostMethod.getResponseBodyAsString().getBytes("ISO 8859-1"),"gb2312");
			String response = postMethod.getResponseBodyAsString(); // 获得结果页面
			System.out.println(response); // 打印返回的结果页面信息
		}
		postMethod.releaseConnection();
	}
}