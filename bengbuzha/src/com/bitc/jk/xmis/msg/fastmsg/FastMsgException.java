package com.bitc.jk.xmis.msg.fastmsg;

import java.util.HashMap;
import java.util.Map;

public class FastMsgException extends Exception {

	private static Map<Integer, String> codeMap = null;
	static {
		codeMap = new HashMap<Integer, String>();
		codeMap.put(0, "成功");
		codeMap.put(1, "未知错误");
		codeMap.put(2, "版本错误");
		codeMap.put(3, "无效参数");
		codeMap.put(4, "已存在");
		codeMap.put(5, "不存在");
		codeMap.put(6, "超时");
		codeMap.put(7, "已登录");
		codeMap.put(8, "未登录");
		codeMap.put(9, "没有账号资源");
		codeMap.put(10, "密码错误");
		codeMap.put(11, "用户冻结");
		codeMap.put(12, "令牌错误");
		codeMap.put(13, "范围超出");
		codeMap.put(14, "没有权限");
	}

	private static final long serialVersionUID = 9074255389134357334L;

	public FastMsgException() {
		super();
	}

	public FastMsgException(int errCode) {
		super("同步FastMsg出错，错误信息：" + codeMap.get(errCode));
	}

	public FastMsgException(String message, Throwable cause) {
		super(message, cause);
	}

	public FastMsgException(String message) {
		super(message);
	}

	public FastMsgException(Throwable cause) {
		super(cause);
	}

}
