package com.bitc.jk.xmis.msg.fastmsg;

import java.net.URL;
import java.rmi.RemoteException;

import com.bitc.jk.xmis.util.CryptUtil;

/**
 * @author bomwu
 * 
 */
public class FastMsg {
	private String serviceUrl;
	private String key;
	private OpenApiBindingStub api;

	public FastMsg(String serviceUrl, String key) throws FastMsgException {
		super();
		this.serviceUrl = serviceUrl;
		this.key = key;
		OpenApi service = new OpenApiLocator();
		try {
			this.api = new OpenApiBindingStub(new URL(this.serviceUrl), service);
		} catch (Exception e) {
			throw new FastMsgException("FastMsg初始化失败");
		}
	}

	public void addDepartment(int departid, String departname, int ordervalue,
			int parentdepartid) throws FastMsgException {
		try {
			int result = api.addDepartment(key, departid, departname,
					ordervalue, parentdepartid);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.addDepartment", e);
		}
	}

	public void removeDepartment(int departid) throws FastMsgException {
		try {
			int result = api.removeDepartment(key, departid);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.removeDepartment", e);
		}
	}

	public void modifyDepartment(int departid, String departname)
			throws FastMsgException {
		try {
			int result = api.modifyDepartment(departname, departid, departname);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyDepartment", e);
		}
	}

	public void setDepartmentOrder(int departid, int ordervalue)
			throws FastMsgException {
		try {
			int result = api.setDepartmentOrder(key, departid, ordervalue);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.setDepartmentOrder", e);
		}
	}

	/**
	 * 
	 * @param userid
	 *            用户ID
	 * @param loginName
	 *            登录名
	 * @param username
	 *            用户名称
	 * @param sex
	 *            性别ID，0女，1男
	 * @param address
	 *            地址
	 * @param telephone
	 *            办公电话
	 * @param mobile
	 *            手机号码
	 * @param email
	 *            电子邮件
	 * @param departid
	 *            所属部门ID
	 * @param departname
	 *            所属部门名称
	 * @param jobtitle
	 *            职务名称
	 * @param password
	 *            密码
	 * @return
	 * @throws FastMsgException
	 */
	public void addUser(int userId, String loginName, String username, int sex,
			String address, String telephone, String mobile, String email,
			int departid, String departname, String jobtitle, String password)
			throws FastMsgException {
		try {
			int result = api.addUser(key, convertUserId(userId), loginName, 1,
					0, username, username, sex, -1, 0, 0, 0, 0, 0, address,
					telephone, mobile, "", "", "", email, "", departid,
					departname, jobtitle, "", "", CryptUtil.MD5(password), "");
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.setDepartmentOrder", e);
		}
	}

	public void removeUser(int userId) throws FastMsgException {
		try {
			int result = api.removeUser(key,
					String.valueOf(convertUserId(userId)));
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.removeUser", e);
		}
	}

	public void modifyUser(int userId, String loginName, String username,
			int sex, String address, String telephone, String mobile,
			String email, int departid, String departname, String jobtitle,
			String password) throws FastMsgException {
		try {
			int result = api.modifyUser(key,
					String.valueOf(convertUserId(userId)), 0, username,
					username, sex, -1, 0, 0, 0, 0, 0, address, telephone,
					mobile, "", "", "", email, "", departid, departname,
					jobtitle, "", "");
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyUser", e);
		}
	}

	public UserInfo getUser(int userId) throws FastMsgException {

		try {
			UserInfo user = api.getUser(key,
					String.valueOf(convertUserId(userId)));
			if (user.getUserid() == 0) {
				throw new FastMsgException("获取用户信息失败。");
			}
			return user;
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.getUser", e);
		}
	}

	private int convertUserId(int userId) {
		return userId + 80000;
	}

	public void changePassword(int userId, String password)
			throws FastMsgException {
		try {
			int result = api.changePassword(key,
					String.valueOf(convertUserId(userId)), CryptUtil.MD5(password));
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyUser", e);
		}
	}

	public UserInfo validateToken(String token) throws FastMsgException {
		try {
			UserInfo result = api.validateToken(key, token);
			if (result.getUserid() == 0) {
				throw new FastMsgException("验证失败。");
			}
			return result;
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyUser", e);
		}
	}

	/**
	 * 发送信息
	 * 
	 * @param from
	 * @param sendto
	 * @param content
	 * @throws FastMsgException
	 */
	public void sendMessage(int from, int sendto, String content)
			throws FastMsgException {
		try {
			int result = api.sendMessage(key,
					String.valueOf(convertUserId(from)),
					String.valueOf(convertUserId(sendto)), content);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyUser", e);
		}
	}

	/**
	 * 发送系统消息
	 * 
	 * @param title
	 *            消息标题
	 * @param content
	 *            消息内容
	 * @param link
	 *            消息链接
	 * @param online
	 *            是否只发给在线用户(0=全部 1=只发给在线用户)
	 * @throws RemoteException
	 */
	public void sendSysMsg(String title, String content, String link, int online)
			throws FastMsgException {
		try {
			// forusertype 指定用户类型(-1=全部 0=外部用户 1=内部用户)
			int result = api.sendSysMsg(key, -1, title, content, link, null,
					online);
			if (result != 0) {
				throw new FastMsgException(result);
			}
		} catch (RemoteException e) {
			throw new FastMsgException("无法同步设置。FastMsg.modifyUser", e);
		}
	}

	public String getServiceUrl() {
		return serviceUrl;
	}

	public void setServiceUrl(String serviceUrl) {
		this.serviceUrl = serviceUrl;
	}

	public String getKey() {
		return key;
	}

	public void setKey(String key) {
		this.key = key;
	}

}
