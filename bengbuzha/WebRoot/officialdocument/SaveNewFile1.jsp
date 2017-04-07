<%@ page language="java"
	import="java.sql.*,java.io.*,com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl,com.bitc.jk.xmis.service.FileUploadDownloadService,com.bitc.jk.xmis.service.SysService,com.bitc.jk.xmis.util.Type,com.bitc.jk.xmis.model.AttachmentFile,com.bitc.jk.xmis.model.EmployeeObj,com.bitc.jk.xmis.util.WebKeys,javax.servlet.*,javax.servlet.http.*,java.text.SimpleDateFormat,java.util.Date,com.zhuozhengsoft.pageoffice.*"
	pageEncoding="utf-8"%>
	<%@page import="com.bitc.jk.xmis.web.SpringInit" %>
	
<%
	//定义保存对象
	FileSaver fs = new FileSaver(request, response);

	
 	String fileName = "正文" + Type.getString(request
		.getParameter("REFID")) + ".doc"; 
	//保存文件
	fs.saveToFile(getServletContext().getAttribute(
			WebKeys.Session_Application_Cfg_AttachedPath)
			  + fileName);
	//设置保存结果
		
		EmployeeObj userDNA = (EmployeeObj) request.getSession()
		.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
		String directPath = (String) getServletContext().getAttribute(
				WebKeys.Session_Application_Cfg_AttachedPath) ;// 在硬盘中的路径
		java.io.File ioFile = new java.io.File(directPath.trim()
				+fileName);

		String messages = "";
		String messageTitle = "";
		InputStream inStream = new FileInputStream(ioFile);
		AttachmentFile attFile = new AttachmentFile(Type
				.getInteger(request.getParameter("type"))
				.intValue());// 2,4,5,6
		FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
		.getBean("fileUploadDownloadServiceImpl",
				FileUploadDownloadServiceImpl.class);
		byte[] content = new byte[fs.getFileSize()];
		attFile.setUploadID(userDNA.getUserID());
		attFile.setDesc(request.getParameter("desc"));
		attFile.setFileName(fileName);
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

		attFile.setFileSize(fs.getFileSize());
		attFile.setFis(inStream);
		boolean flag1 = service.doSaveAttachment(attFile,true);// 保存
		if (flag1) {
			fs.setCustomSaveResult("ok");
		} else {}
	//fs.showPage(300,300);
	fs.close();
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>

		<title>My JSP 'SaveNewFile.jsp' starting page</title>

		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">

	</head>

	<body>
		<br>
	</body>
</html>
