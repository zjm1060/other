package com.bitc.jk.xmis.web;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.service.impl.ContractServiceImpl;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.service.impl.KZHBServiceImpl;
import com.bitc.jk.xmis.service.impl.OrganizationServiceImpl;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.DuplicatedException;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.jspsmart.upload.File;
import com.jspsmart.upload.SmartUpload;
import com.jspsmart.upload.SmartUploadException;
import com.zhuozhengsoft.pageoffice.FileSaver;

public class FileUploadDownload extends HttpServlet {

	private static final long serialVersionUID = 7931867773047468306L;
	private static Logger logger = Logger.getLogger(FileUploadDownload.class);

	private SysService sysService;

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

		case WebKeys.ServiceImpl_Object_KZHB:

			serviceImplObj = (KZHBServiceImpl) context
					.getBean("kzhbServiceImpl");
			break;
		}

		return serviceImplObj;
	}

	public void service(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {

		logger.info("-----------> HttpServlet &type="
				+ request.getParameter("type"));
		String methodName = Type.getString(request.getParameter("method"));
		if (StringUtils.isNotEmpty(methodName)) {
			Method method;
			try {
				method = FileUploadDownload.class.getMethod(methodName,
						HttpServletRequest.class, HttpServletResponse.class);
			} catch (Exception e) {
				throw new ServletException("reflect error!", e);
			}

			if (method != null) {
				try {
					method.invoke(this, request, response);
				} catch (Exception e) {
					throw new ServletException("method(" + methodName
							+ ") invoke error!", e);
				}
				return;
			}
		}
		int type = Type.getInteger(request.getParameter("type")).intValue();
		switch (type) {
		case WebKeys.Upload_Application_UserPhoto_Type:
			doUploadUserPic(request, response);
			break;
		case WebKeys.Upload_Application_Download_Type:
			doDownloadAttachment(request, response);
			break;
		case WebKeys.Upload_Application_Delete_Type:
			doDeleteAttachment(request, response);
			break;
		case WebKeys.Upload_Application_GetUserPic_Type:
			doGetUserPic(request, response);
			break;
		case WebKeys.Upload_Application_DOWNLOADTEMPFILE_Type:
			doDownloadTempfile(request, response);
			break;
		case WebKeys.Upload_Application__ExistValidate:
			existValidate(request, response);
			break;
		case WebKeys.Upload_Application__deleteByConIDFileName:
			deleteByREFIDFileName(request, response);
			break;
		case WebKeys.Upload_Application_DOWNLOADTEMPFILEZW:
			downDocument(request, response);
			break;
		default:// 其他的都调用上传，包括type = 2,4,5,6
			// doUploadAttachment(request,response);
			doUploadAttachmentNew(request, response);
			break;
		}
	}

	public void doUploadUserPic(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		logger.info("---> pic upload..." + request.getParameter("userid"));
		String filePath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath)
				+ "userPic";
		//增加一个判断名片的标识位的  isPostCard=front back
		if("front".equals(request.getParameter("isPostCard"))){
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "postCardFrontPic";
		}else if("back".equals(request.getParameter("isPostCard"))){
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "postCardBackPic";
		}else if("deviceImg".equals(request.getParameter("isPostCard"))){
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "deviceImg";
		}

		String messages = "";
		String messageTitle = "";
		String picURL = "";
		SmartUpload su = new SmartUpload();
		su.setCharSet("utf-8");
		long maxsize = 10 * 1024 * 1024; // 设置每个上传文件的大小，为2MB
		String allowedFilesList = "jpg,gif,bmp,png,JPG,GIF,BMP,PNG";
		String denidFilesList = "exe,bat,jsp,htm,html,,";

		String photoAddr = "";
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		EmployeeObj employeeObj = WebUtil.getCurrentEmployee(request
				.getSession(false));
		try {
			new java.io.File(filePath).mkdirs();
			su.initialize(this.getServletConfig(), request, response); // 初始化
			su.setMaxFileSize(maxsize); // 限制上传文件的大小
			su.setAllowedFilesList(allowedFilesList); // 设置允许上传的文件类型
			su.setDeniedFilesList(denidFilesList);

			su.upload(); // 上传文件

			File file = su.getFiles().getFile(0); // 获取上传的文件，因为只上传了一个文件，所以可直接获取

			if (!file.isMissing()) { // 如果选择了文件

				logger.info(file.getFileName());
				picURL = request.getParameter("userid") + "."
						+ file.getFileExt();
				// photoAddr = filePath + now + "." + file.getFileExt(); //
				// filePath值
				photoAddr = filePath + "/" + picURL;

				file.saveAs(photoAddr, File.SAVEAS_PHYSICAL);
				messages = "图片上传成功";
				messageTitle = "成功";

				// picURL = WebKeys.AttachmentURL
				// + request.getParameter("userid") + "."
				// + file.getFileExt();

				OrganizationServiceImpl o = (OrganizationServiceImpl) this
						.getServiceImplObj(WebKeys.ServiceImpl_Object_Organization);

				int result = 0;
				if("".equals(request.getParameter("isPostCard"))){
					result = o.doSaveUserPhoto(
							Integer.parseInt(request.getParameter("userid")),
							request.getParameter("userid") + "."
									+ file.getFileExt(), employeeObj.getUserID()) ;
				}

				
				//增加一个判断名片的标识位的  isPostCard=front back
				if("front".equals(request.getParameter("isPostCard"))){
					result = o.doSavePostCardFrontPic(
							Integer.parseInt(request.getParameter("userid")),
							request.getParameter("userid") + "."
									+ file.getFileExt(), employeeObj.getUserID());
				}
				if("back".equals(request.getParameter("isPostCard"))){
					result = o.doSavePostCardBackPic(
							Integer.parseInt(request.getParameter("userid")),
							request.getParameter("userid") + "."
							+ file.getFileExt(), employeeObj.getUserID());
				}
				if("deviceImg".equals(request.getParameter("isPostCard"))){
					System.out.println("deviceImg------------------------------");
					result = o.doSavePostMaterials(
							request.getParameter("userid"),
							request.getParameter("userid")+ "." + file.getFileExt());
					System.out.println(result+"----");
				}
				if (result>0) {
					messages = "图片上传成功";
					messageTitle = "成功";
					ajaxResponse.setSuccess(true);
					ajaxResponse.addAttribute("picPath", picURL);
				} else {
					ajaxResponse.setSuccess(false);
					messages = "图片上传失败";
					messageTitle = "失败";
				}

			} else {
				ajaxResponse.setSuccess(false);
				messages = "请选择要上传的文件！";
				messageTitle = "提示";
			}

		} catch (java.lang.SecurityException e) {
			ajaxResponse.setSuccess(false);
			messageTitle = "上传文件失败";

			if (e.getMessage().indexOf("(1105)") > 0)
				messages = "图片大小需小于10M";
			else if (e.getMessage().indexOf("(1010)") > 0)
				messages = "图片类型只允许为：JPG,GIF,BMP,PNG";
		} catch (SmartUploadException e) {
			ajaxResponse.setSuccess(false);
			messages = "上传文件失败！";
			messageTitle = "上传文件失败";
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			e.printStackTrace();
			messages = "上传文件失败！请重新登录后再操作，谢谢";
			messageTitle = "上传文件失败";

		}
		ajaxResponse.setMessage(messages);
		ajaxResponse.setMessageTitle(messageTitle);
		logger.info(messages);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString(),
				"text/html; charset=utf-8");
	}

	/**
	 * 上传附件
	 * 
	 * @param request
	 * @param response
	 * @return
	 */
	public void doUploadAttachmentNew(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		logger.info("---> attachment upload... type="
				+ request.getParameter("type"));
		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath)
				+ request.getParameter("type") + "\\";// 在硬盘中的路径

		java.io.File path = new java.io.File(directPath);
		path.mkdirs();

		String messages = "";
		String messageTitle = "";
		int REFID = 0;

		SmartUpload su = new SmartUpload();
		su.setCharSet("utf-8");

		long maxsize = 6 * 1024 * 1024; // 设置每个上传文件的大小，为6MB

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

				su.upload(); // 上传文件
				File file = su.getFiles().getFile(0); // 获取上传的文件(只上传了一个文件,可直接获取)

				if (!file.isMissing()) { // 如果选择了文件
					FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
							.getBean("fileUploadDownloadServiceImpl",
									FileUploadDownloadServiceImpl.class);
					AttachmentFile attFile = new AttachmentFile(Type
							.getInteger(request.getParameter("type"))
							.intValue());// 2,4,5,6
					// attFile.setFileName(file.getFileName());
					// attFile.setCoerciveFlag(Type.getBoolean(request
					// .getParameter("coerciveFlag")));

					file.saveAs(directPath + file.getFileName(),
							File.SAVEAS_PHYSICAL);// 保存文件，没有返回值
					java.io.File ioFile = new java.io.File(directPath.trim()
							+ file.getFileName());

					InputStream inStream = new FileInputStream(ioFile);

					byte[] content = new byte[file.getSize()];
					attFile.setUploadID(userDNA.getUserID());
					attFile.setDesc(request.getParameter("desc"));
					attFile.setFileName(file.getFileName());
					attFile.setContent(content);
					attFile.setAttachmentTypeID(Type.GetInt(request
							.getParameter("attachmentTypeID")));
					attFile.setRefID(Type.getString(request
							.getParameter("REFID")));
					if (Type.getInteger(request.getParameter("type"))
							.intValue() == WebKeys.Upload_Application_KzhbFile_Type) {
						attFile.setAttachmentTypeID(Type.GetInt(-1));
						attFile.setRefID(Type.getString(-1));
					}
					attFile.setKeyWords(request.getParameter("keywords"));

					attFile.setFilePath(directPath);

					attFile.setFileSize(file.getSize());
					attFile.setFis(inStream);
					boolean flag1 = service.doSaveAttachment(attFile,false);// 保存
					if (flag1) {
						messages = "附件上传成功";
						messageTitle = "成功";
						flag1 = true;
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
				messages = "附件类型只允许为："
						+ WebKeys.Upload_File_Type_Err_Msg_Contract;
			else if (e.getMessage().indexOf("(1015)") > 0)
				// messages = "附件类型不允许：exe,bat,jsp,htm,html";
				messages = "附件类型只允许为："
						+ WebKeys.Upload_File_Type_Err_Msg_Contract;
		} catch (SmartUploadException e) {
			e.printStackTrace();
			messageTitle = "上传文件失败！";
			messages = "上传文件失败，请重新操作！";
		} catch (DuplicatedException e) {
			e.printStackTrace();
			messageTitle = "上传文件已存在";
			messages = e.getMessage();
		} catch (Exception e) {
			e.printStackTrace();
			messages = "上传文件失败！请重新登录后再操作，谢谢";
			messageTitle = "上传文件失败";
		}

		logger.info(messageTitle + ":" + messages);

		try {
			// 返回成功标识
			ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("messages", messages);
			ajaxResponse.addAttribute("messageTitle", messageTitle);
			ajaxResponse.addAttribute("REFID", String.valueOf(REFID));
			WebUtil.returnResponse(response, ajaxResponse.toJsonString(),
					"text/html; charset=utf-8");

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

		// String directPath = request.getRealPath("/ ").trim()
		// + WebKeys.AttachmentWebPath;
		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath);
		logger.info("directPath=" + directPath);
		// logger.info("start 1= " + new
		// java.sql.Time(System.currentTimeMillis()).toString());
		// String fileName = ((ContractServiceImpl)
		// getServiceImplObj(WebKeys.ServiceImpl_Object_Contract))
		// .getAttachmentByID(Type.GetInt(request.getParameter("attID")),
		// directPath);
		FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
				.getBean("fileUploadDownloadServiceImpl",
						FileUploadDownloadServiceImpl.class);
		String fileName = service.getAttachmentByID(
				Type.GetInt(request.getParameter("attID")), directPath);
		logger.info("fileName=" + fileName);

		// fileName = fileName.substring(4);

		// logger.info("start 2= " + new
		// java.sql.Time(System.currentTimeMillis()).toString());

		try {
			// download with smartupload
			SmartUpload sua = new SmartUpload();
			sua.initialize(this.getServletConfig(), request, response);
			sua.setContentDisposition(null); // 设定contentDisposition为null以禁止浏览器自动打开文件,保证点击链接后是下载文件
			sua.downloadFile(fileName, "application/x-msdownload");

		} catch (Exception e) {
			e.printStackTrace();
			// messages = "上传文件失败！请重新登录后再操作，谢谢";
			// messageTitle = "上传文件失败";

		}

		// logger.info("start 3= " + new
		// java.sql.Time(System.currentTimeMillis()).toString());

	}

	public void doDownloadTempfile(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, UnsupportedEncodingException {
		String key = request.getParameter("key");
		String fileName =java.net.URLDecoder.decode( request.getParameter("fileName"),"utf-8");
		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_TEMPFILEPATH);
		String filePath = directPath + key;
		try {
			SmartUpload sua = new SmartUpload();
			sua.initialize(this.getServletConfig(), request, response);
			sua.setContentDisposition(null); // 设定contentDisposition为null以禁止浏览器自动打开文件,保证点击链接后是下载文件
			sua.downloadFile(filePath, "application/x-msdownload",
					fileName == null ? key : fileName);
			new java.io.File(filePath).delete();
		} catch (Exception e) {
			logger.error("下载临时文件失败", e);
		}
	}

	/**
	 * 删除附件
	 * 
	 * @param request
	 * @param response
	 * @return
	 */
	public void doDeleteAttachment(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		logger.info("---> attchment delete..."
				+ request.getParameter("AttachementID"));
		int AttachementID = Type.GetInt(request.getParameter("AttachementID"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			service.doDeleteAttachementById(AttachementID);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
		}
		response.getWriter().write(ajaxResponse.toString());
	}

	public void doGetUserPic(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		logger.info("doGetUserPic--->userPic="
				+ request.getParameter("userPic")+"---------标志位："+request.getParameter("isPostCard"));
		String userPic = request.getParameter("userPic");
		String filePath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath)
				+ "userPic";
		//增加一个判断名片的标识位的  isPostCard=front back
		if("front".equals(request.getParameter("isPostCard"))){
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "postCardFrontPic";
		}
		if("back".equals(request.getParameter("isPostCard"))){
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "postCardBackPic";
		}else if("deviceImg".equals(request.getParameter("isPostCard"))){
			System.out.println("isPostCard+++++++++++++++++++++++=");
			filePath = (String) getServletContext().getAttribute(
					WebKeys.Session_Application_Cfg_AttachedPath)
					+ "deviceImg";
		}
		
		String photoAddr = filePath +"/"+ userPic;
		System.out.println("photoAddr----"+photoAddr);
		java.io.File photoAddrFile= new java.io.File(photoAddr);
		if(photoAddrFile==null || !photoAddrFile.exists()){
			//photoAddrFile=一个指定存在的默认图片；
			photoAddrFile.createNewFile();
		}
		FileInputStream hFile = new FileInputStream(photoAddrFile); // 以byte流的方式打开文件
		int i = hFile.available(); // 得到文件大小
		byte data[] = new byte[i];
		hFile.read(data); // 读数据
		hFile.close();
		response.setContentType("image/*"); // 设置返回的文件类型
		OutputStream toClient = response.getOutputStream(); // 得到向客户端输出二进制数据的对象
		toClient.write(data); // 输出数据
		toClient.close();
	}

	/**
	 * 提示上传文件已存在时（覆盖/取消）
	 * 
	 * @param request
	 * @param response
	 * @return
	 */
	public void existValidate(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		int attachmentTypeID = Type.GetInt(request
				.getParameter("attachmentTypeID"));
		String REFID = Type.getString(request.getParameter("REFID"));
		String fileName = java.net.URLDecoder.decode(
				request.getParameter("fileName"), "UTF-8");
		int type = Type.GetInt(request
				.getParameter("type"));
		int index = fileName.lastIndexOf("\\");
		if (index > 0) {
			fileName = fileName.substring(index + 1);
		}
		logger.info("附件覆盖(判断是否已存在)--->attachmentTypeID=" + attachmentTypeID
				+ "  REFID=" + REFID + "  fileName=" + fileName);

		FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
				.getBean("fileUploadDownloadServiceImpl",
						FileUploadDownloadServiceImpl.class);
		int number = service.existValidate(attachmentTypeID, REFID, fileName,type);
		Boolean flag = number > 0;// 是否存在标记
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setSuccess(true);
		ajaxResponse.addAttribute("flag", flag);
		ajaxResponse.addAttribute("fileName", fileName);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());

	}

	/**
	 * 删除附件
	 * 
	 * @param request
	 * @param response
	 * @return
	 */
	public void deleteByREFIDFileName(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {
		int attachmentTypeID = Type.GetInt(request
				.getParameter("attachmentTypeID"));
		String REFID = Type.getString(request.getParameter("REFID"));
		String fileName = java.net.URLDecoder.decode(
				request.getParameter("fileName"), "UTF-8");
		logger.info("附件覆盖(上传文件已存在时删除原有的)--->attachmentTypeID="
				+ attachmentTypeID + "  REFID=" + REFID + "  fileName="
				+ fileName);
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			service.doDeleteByConIDFileName(attachmentTypeID, REFID, fileName);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
		}
		response.getWriter().write(ajaxResponse.toString());

	}

	/**
	 * 获取附件列表
	 * 
	 * @param request
	 * @param response
	 * @return
	 */
	public void getAttachmentList(HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("getAttachmentList--->REFID="
				+ Type.getString(request.getParameter("REFID"))
				+ "   attachmentTypeID="
				+ Type.GetInt(request.getParameter("attachmentTypeID")));
		try {
			String REFID = Type.getString(request.getParameter("REFID"));
			int attachmentTypeID = Type.GetInt(request
					.getParameter("attachmentTypeID"));
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			String store = service.getAttachmentList(REFID, attachmentTypeID);
			logger.info(store);
			if(Type.GetInt(request.getParameter("isApp"))== 1 ){
				response.setContentType("application/json;charset=UTF-8");
				response.getWriter().write(store.toString());
			}else{
			WebUtil.returnResponse(response, store);
			}

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

	}

	/**
	 * 获取附件列表 科质部
	 * 
	 */
	public void getAttachmentListForKZHB(HttpServletRequest request,
			HttpServletResponse response) {
		String store = "";
		try {
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			PageInfo pageInfo = WebUtil.generatePageInfo(request);

			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null) {
				querys = new ArrayList<StandardQuery>();
			}
			List<StandardQuery> query2 = WebUtil.generateStandardQuerysByMulit(
					request, sysService);
			if (query2 != null) {
				for (int i = 0; i < query2.size(); i++) {
					querys.add(query2.get(i));
				}
			}
			store = service.getAttachmentListForKZHB(querys, pageInfo);
			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

	}

	/**
	 * 删除科质部 文件 逻辑删除 将删除标志 置2
	 * */
	public void deleteKzhbFileById(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		int attID = Type.GetInt(request.getParameter("attachmentID"));
		logger.info("科质部文件逻辑删除      附件ID=" + attID);
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			service.doDeleteKzhbFileById(attID);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
		}
		response.getWriter().write(ajaxResponse.toString());

	}

	public SysService getSysService() {
		return sysService;
	}

	public void setSysService(SysService sysService) {
		this.sysService = sysService;
	}
	public void downDocument(HttpServletRequest request,
			HttpServletResponse response) throws IOException{
		String formId = request.getParameter("form_id");
		String name = request.getParameter("fileName");
		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath);
		String fileName = directPath +name+formId+".doc";
		java.io.File f = new java.io.File(fileName);
		if(f.exists()){
			try {
				
				SmartUpload sua = new SmartUpload();
				sua.initialize(this.getServletConfig(), request, response);
				sua.setContentDisposition(null); // 设定contentDisposition为null以禁止浏览器自动打开文件,保证点击链接后是下载文件
				sua.downloadFile(fileName, "application/x-msdownload",name+".doc");
			} catch (Exception e) {
				logger.error("下载文件失败", e);
			}
		}else{
			response.setContentType("text/html;charset=UTF-8");
			response.getWriter().write("<script type=text/javascript>history.back();alert('该行文不存在！');</script>");
		}
		
	}

}
