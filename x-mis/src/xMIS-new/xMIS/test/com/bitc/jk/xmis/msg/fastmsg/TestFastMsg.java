package com.bitc.jk.xmis.msg.fastmsg;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.rmi.RemoteException;
import java.security.NoSuchAlgorithmException;

import com.bitc.jk.xmis.util.CryptUtil;

public class TestFastMsg {

	public static void testGetUserInfo() throws FastMsgException {
		FastMsg fm = new FastMsg(
				"http://xmis.bitc.net.cn:5880/openapi/openapi.php",
				"5DEDD10D2E434A139A05953BDB66CC68");
		UserInfo user = fm.getUser(662);
		System.out.println(user.getActualname());
		fm.sendMessage(662, 1029, "哈大家好");
	}

	/**
	 * @param args
	 * @throws FastMsgException
	 * @throws UnsupportedEncodingException
	 * @throws NoSuchAlgorithmException
	 * @throws RemoteException
	 * @throws MalformedURLException
	 */
	public static void main(String[] args) throws FastMsgException,
			NoSuchAlgorithmException, UnsupportedEncodingException {
		// testGetUserInfo();
		System.out.println(CryptUtil.MD5("111111"));
	}
}
