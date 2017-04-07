<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<%@page import="com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl"%>
<%@ page language="java"
	import="com.zhuozhengsoft.pageoffice.*,java.util.*,java.io.*,javax.servlet.*,javax.servlet.http.*;"
	pageEncoding="gb2312"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
	String readOnly = Type.getString(request.getAttribute("readOnly"));
	System.out.print("33333"+readOnly);

	//String filePath = request.getSession().getServletContext().getRealPath("SimpleWord/doc/test.doc");
	String filePath = (String) request.getAttribute("filepath");
	String ext = filePath.substring(filePath.lastIndexOf("."),
			filePath.length()).toLowerCase();

	//******************************卓正PageOffice组件的使用*******************************

	PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
	poCtrl1.setServerPage(request.getContextPath() + "/poserver.zz"); //此行必须
	poCtrl1.setSaveFilePage(path + "/weboffice.do?method=saveFile");//如要保存文件，此行必须
	poCtrl1.addCustomToolButton("保存", "Save()", 1);//添加自定义工具栏按钮
	poCtrl1.addCustomToolButton("全屏", "SetFullScreen()", 4);
	//打开文件
	if ("".equals(readOnly)) {
		if (".doc".equals(ext) || ".docx".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.docNormalEdit,
					WebUtil.getCurrentEmployee().getUserName());
		} else if (".xls".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.xlsNormalEdit,
					WebUtil.getCurrentEmployee().getUserName());
		} else if (".ppt".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.pptNormalEdit,
					WebUtil.getCurrentEmployee().getUserName());
		}

	} else {
		if (".doc".equals(ext) || ".docx".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.docReadOnly,
					WebUtil.getCurrentEmployee().getUserName());
		} else if (".xls".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.xlsReadOnly,
					WebUtil.getCurrentEmployee().getUserName());
		} else if (".ppt".equals(ext)) {
			poCtrl1.webOpen(filePath, OpenModeType.pptReadOnly,
					WebUtil.getCurrentEmployee().getUserName());
		}
	}
	poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须

	String attIDParams = (String) request.getAttribute("attIDParams");
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<base href="<%=basePath%>">

<title>文件编辑</title>
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
<meta http-equiv="description" content="This is my page">
<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
</head>

<body>
<script type="text/javascript">
	function Save() {
		document.getElementById("PageOfficeCtrl1").WebSave();
	}
	function SetFullScreen() {
		document.getElementById("PageOfficeCtrl1").FullScreen = !document
				.getElementById("PageOfficeCtrl1").FullScreen;
	}
</script>
<form id="form1"><input name="token" type="hidden"
	value="<%=request.getAttribute("filetoken")%>" /></form>

<po:PageOfficeCtrl id="PageOfficeCtrl1" />
</body>
</html>
