package com.bitc.jk.xmis.web;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.impl.ContractServiceImpl;
import com.bitc.jk.xmis.service.impl.OrganizationServiceImpl;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.jspsmart.upload.File;
import com.jspsmart.upload.SmartUpload;
import com.jspsmart.upload.SmartUploadException;

public class FileUploadDownload extends HttpServlet implements Serializable {

	private static Logger logger = Logger.getLogger(FileUploadDownload.class);

	private Object getServiceImplObj(int serviceName) {

		ApplicationContext context = WebApplicationContextUtils
				.getWebApplicationContext(getServletContext());

		Object serviceImplObj = new Object();
		switch (serviceName) {
		case WebKeys.ServiceImpl_Object_Organization:
			serviceImplObj = (OrganizationServiceImpl) context
					.getBean("organizationServiceImpl");
			break;
		case WebKeys.ServiceImpl_Object_Contract:
			serviceImplObj = (ContractServiceImpl) context
					.getBean("contractServiceImpl");
			break;
		}

		return serviceImplObj;
	}

	public void service(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		logger.info("-----------> HttpServlet &type="
				+ request.getParameter("type") );
		int type = Type.getInteger(request.getParameter("type")).intValue();
		switch (type) {
		case WebKeys.Upload_Application_UserPhoto_Type:
			doUploadUserPic(request, response);
			break;
		case WebKeys.Upload_Application_Download_Type:
			doDownloadAttachment(request, response);
			break;
		default:
			doUploadContractAttachment(request, response);
			break;
		}
	}

	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		// this.doPost(request, response);
	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
	}

	public void doUploadUserPic(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		logger.info("---> pic upload..." + request.getParameter("userid"));

		String filePath = WebKeys.AttachmentWebPath;
		String messages = "";
		String messageTitle = "";
		String picURL = "";
		boolean flag = false;

		SmartUpload su = new SmartUpload();
		su.setCharSet("utf-8");
		long maxsize = 2 * 1024 * 1024; // 设置每个上传文件的大小，为2MB
		String allowedFilesList = "jpg,gif,bmp,png";
		String denidFilesList = "exe,bat,jsp,htm,html,,";

		String photoAddr = "";

		try {
			EmployeeObj userDNA = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userDNA == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {

				su.initialize(this.getServletConfig(), request, response); // 初始化
				su.setMaxFileSize(maxsize); // 限制上传文件的大小
				su.setAllowedFilesList(allowedFilesList); // 设置允许上传的文件类型
				su.setDeniedFilesList(denidFilesList);

				su.upload(); // 上传文件

				File file = su.getFiles().getFile(0); // 获取上传的文件，因为只上传了一个文件，所以可直接获取

				if (!file.isMissing()) { // 如果选择了文件

					logger.info(file.getFileName());

					String now = new Date().getTime() + ""; // 获取当前时间并格式化为字符串
					// photoAddr = filePath + now + "." + file.getFileExt(); //
					// filePath值
					photoAddr = filePath + request.getParameter("userid") + "."
							+ file.getFileExt();

					file.saveAs(photoAddr, File.SAVEAS_VIRTUAL);
					messages = "图片上传成功";
					messageTitle = "成功";

					picURL = WebKeys.AttachmentURL
							+ request.getParameter("userid") + "."
							+ file.getFileExt();

					OrganizationServiceImpl o = (OrganizationServiceImpl) this
							.getServiceImplObj(WebKeys.ServiceImpl_Object_Organization);

					if (o.doSaveUserPhoto(Integer.parseInt(request
							.getParameter("userid")), request
							.getParameter("userid")
							+ "." + file.getFileExt(), userDNA.getUserID()) > 0) {
						messages = "图片上传成功";
						messageTitle = "成功";
						flag = true;
					} else {
						messages = "图片上传失败";
						messageTitle = "失败";
					}

				} else {
					messages = "请选择要上传的文件！";
					messageTitle = "提示";
				}
			}

		} catch (java.lang.SecurityException e) {
			messageTitle = "上传文件失败";

			if (e.getMessage().indexOf("(1105)") > 0)
				messages = "图片大小需小于2M";
			else if (e.getMessage().indexOf("(1010)") > 0)
				messages = "图片类型只允许为：JPG,GIF,BMP,PNG";
		} catch (SmartUploadException e) {
			messages = "上传文件失败！";
			messageTitle = "上传文件失败";
		} catch (Exception e) {
			e.printStackTrace();
			messages = "上传文件失败！请重新登录后再操作，谢谢";
			messageTitle = "上传文件失败";

		}

		logger.info(messages);

		// request.setAttribute("messages", messages);
		// request.getRequestDispatcher(forward).forward(request, response);

		// JSONObject jObject = new JSONObject();

		// try {
		// jObject.put("success", true);
		// } catch (Exception e) {
		// e.printStackTrace();
		// }
		// response.getWriter().print(jObject.toString());
		try {

			// 返回成功标识
			// response.getWriter().write(jObject.toString());
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(
					"{success:" + String.valueOf(flag) + ",message:'"
							+ messages + "',messageTitle:'" + messageTitle
							+ "',picPath:'" + picURL + "'}");
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public void doUploadContractAttachment(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		logger.info("---> attchment upload..." + request.getParameter("conID"));

		String directPath = request.getRealPath("/ ");

		logger.info(directPath);

		String messages = "";
		String messageTitle = "";
		int contractID=0;
		boolean flag = false;

		SmartUpload su = new SmartUpload();
		su.setCharSet("utf-8");

		long maxsize = 6 * 1024 * 1024; // 设置每个上传文件的大小，为6MB
		String allowedFilesList = WebKeys.Upload_File_Type_Allowed_Contract;
		String denidFilesList = WebKeys.Upload_File_Type_Denfid_Contract;

		String photoAddr = "";

		try {
			EmployeeObj userDNA = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userDNA == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {

				su.initialize(this.getServletConfig(), request, response); // 初始化
				su.setMaxFileSize(maxsize); // 限制上传文件的大小
				su.setAllowedFilesList(allowedFilesList); // 设置允许上传的文件类型
				su.setDeniedFilesList(denidFilesList);

				su.upload(); // 上传文件
				
				File file = su.getFiles().getFile(0); // 获取上传的文件，因为只上传了一个文件，所以可直接获取

				if (!file.isMissing()) { // 如果选择了文件

					photoAddr = WebKeys.AttachmentWebPath + file.getFileName();
					file.saveAs(photoAddr, File.SAVEAS_VIRTUAL);
					java.io.File ioFile = new java.io.File(directPath.trim()
							+ photoAddr);
					InputStream inStream = new FileInputStream(ioFile);
					
					byte[] content = new byte[file.getSize()];

					AttachmentFile attFile = new AttachmentFile();
					attFile
							.setRefID(Type
									.GetInt(su.getRequest().getParameter("conID")));
					
					attFile.setRefType(Type.GetInt(su.getRequest().getParameter("conType")));
					attFile.setRefName(Type.getString(su.getRequest().getParameter("conName")));
					
					attFile.setFileName(file.getFileName());
					attFile.setFileSize(file.getSize());
					attFile.setContent(content);
					attFile.setUploadID(userDNA.getUserID());
					attFile.setFis(inStream);

					ContractServiceImpl contractService = (ContractServiceImpl) getServiceImplObj(WebKeys.ServiceImpl_Object_Contract);

					switch(Type.getInteger(request.getParameter("type")).intValue()){
					case WebKeys.Upload_Application_Contract_Type:
						contractID = contractService.doSaveAttachmentForContract(attFile);
						break;
					case WebKeys.Upload_Application_PaymentInfo_Type:
						contractID = contractService.doSaveAttachmentForPayment(attFile);
						break;

					case WebKeys.Upload_Application_InvoiceInfo_Type:
						contractID = contractService.doSaveAttachmentForInvoice(attFile);
						break;
					}
					
//					inStream.close();
					if (contractID > 0) {
						messages = "附件上传成功";
						messageTitle = "成功";
						flag = true;
					} else {
						messages = "附件上传失败";
						messageTitle = "失败";
					}
				} else {
					messages = "请选择要上传的文件！";
					messageTitle = "提示";
				}

			}

		} catch (java.lang.SecurityException e) {
			e.printStackTrace();
			// messages = "附件类型只允许为：JPG, GIF, BMP, PNG；且图片尺寸小于6M";
			messageTitle = "上传文件失败";

			if (e.getMessage().indexOf("(1105)") > 0)
				messages = "附件大小需小于6M";
			else if (e.getMessage().indexOf("(1010)") > 0)
				messages = "附件类型只允许为："+WebKeys.Upload_File_Type_Err_Msg_Contract;
			else if (e.getMessage().indexOf("(1015)") > 0)
				//messages = "附件类型不允许：exe,bat,jsp,htm,html";
				messages = "附件类型只允许为："+WebKeys.Upload_File_Type_Err_Msg_Contract;
			
		} catch (SmartUploadException e) {
			messages = "上传文件失败！";
			messageTitle = "上传文件失败，请重新操作！";
		} catch (Exception e) {
			e.printStackTrace();
			messages = "上传文件失败！请重新登录后再操作，谢谢";
			messageTitle = "上传文件失败";

		}

		logger.info(messages);

		// request.setAttribute("messages", messages);
		// request.getRequestDispatcher(forward).forward(request, response);

		// JSONObject jObject = new JSONObject();

		// try {
		// jObject.put("success", true);
		// } catch (Exception e) {
		// e.printStackTrace();
		// }
		// response.getWriter().print(jObject.toString());
		try {

			// 返回成功标识
			// response.getWriter().write(jObject.toString());
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(
					"{success:" + String.valueOf(flag) + ",message:'"
							+ messages + "',messageTitle:'" + messageTitle
							+ "',contractID:'" + String.valueOf(contractID) 
							//+ "', attTypeID:'" + String.valueOf(attTypeID)
							+ "'}");

		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public void doDownloadAttachment(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		logger.info("---> attchment download..."
				+ request.getParameter("attID"));
		
		String directPath = request.getRealPath("/ ").trim() + WebKeys.AttachmentWebPath ;;
		
//		logger.info("start 1= " + new java.sql.Time(System.currentTimeMillis()).toString());
		String fileName = ((ContractServiceImpl) getServiceImplObj(WebKeys.ServiceImpl_Object_Contract))
				.getAttachmentByID(Type.GetInt(request.getParameter("attID")), directPath);


		logger.info(fileName);

//		logger.info("start 2= " + new java.sql.Time(System.currentTimeMillis()).toString());

		try{
			// download with smartupload
			SmartUpload sua = new SmartUpload();
			sua.initialize(this.getServletConfig(), request, response);
			sua.setContentDisposition(null); // 设定contentDisposition为null以禁止浏览器自动打开文件,保证点击链接后是下载文件
			sua.downloadFile(fileName, "application/x-msdownload");
			
		} catch (Exception e) {
			e.printStackTrace();
//			messages = "上传文件失败！请重新登录后再操作，谢谢";
//			messageTitle = "上传文件失败";

		}
		
//		logger.info("start 3= " + new java.sql.Time(System.currentTimeMillis()).toString());
		
	}


}
