package com.bitc.jk.xmis.util;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

/**
 * @author Bom Wu
 * 
 */
public class CryptUtil {

	private static final String UTF_8 = "utf-8";

	/**
	 * Base64解码
	 * 
	 * @param key
	 * @return
	 * @throws IOException
	 * @throws UnsupportedEncodingException
	 */
	public static String decryptBase64(String key)
			throws UnsupportedEncodingException, IOException {
		return new String(new BASE64Decoder().decodeBuffer(key), UTF_8);
	}

	/**
	 * Base64编码
	 * 
	 * @param bytes
	 * @return
	 * @throws UnsupportedEncodingException
	 */
	public static String encryptBase64(byte[] bytes) {
		return new BASE64Encoder().encode(bytes);
	}

	/**
	 * Base64编码
	 * 
	 * @param key
	 * @return
	 * @throws UnsupportedEncodingException
	 */
	public static String encryptBase64(String key)
			throws UnsupportedEncodingException {
		return encryptBase64(key.getBytes(UTF_8));
	}

	public static String MD5_BASE64(String key) throws NoSuchAlgorithmException,
			UnsupportedEncodingException {
		MessageDigest md5 = MessageDigest.getInstance("md5");
		md5.update(key.getBytes(UTF_8));
		return encryptBase64(md5.digest());
	}
	
	 public final static String MD5(String s) {
	        char hexDigits[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};       
	        try {
	            byte[] btInput = s.getBytes();
	            // 获得MD5摘要算法的 MessageDigest 对象
	            MessageDigest mdInst = MessageDigest.getInstance("MD5");
	            // 使用指定的字节更新摘要
	            mdInst.update(btInput);
	            // 获得密文
	            byte[] md = mdInst.digest();
	            // 把密文转换成十六进制的字符串形式
	            int j = md.length;
	            char str[] = new char[j * 2];
	            int k = 0;
	            for (int i = 0; i < j; i++) {
	                byte byte0 = md[i];
	                str[k++] = hexDigits[byte0 >>> 4 & 0xf];
	                str[k++] = hexDigits[byte0 & 0xf];
	            }
	            return new String(str);
	        } catch (Exception e) {
	            e.printStackTrace();
	            return null;
	        }
	    } 
}
