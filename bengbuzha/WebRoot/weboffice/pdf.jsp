<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@ page language="java"
	import="com.zhuozhengsoft.pageoffice.*,java.util.*,java.io.*,javax.servlet.*,javax.servlet.http.*;"
	pageEncoding="gb2312"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";

	//String filePath = request.getSession().getServletContext().getRealPath("SimpleWord/doc/test.doc");
	String filePath = (String) request.getAttribute("filepath");
	String ext = filePath.substring(filePath.lastIndexOf("."),
			filePath.length()).toLowerCase();

	//******************************卓正PageOffice组件的使用*******************************

	if (".pdf".equals(ext)) {
		PDFCtrl pdfCtr = new PDFCtrl(request);
		pdfCtr.setServerPage("poserver.do");//此行必须
		pdfCtr.addCustomToolButton("全屏", "SetFullScreen()", 4);
		pdfCtr.webOpen(filePath);
		pdfCtr.setTagId("PDFCtrl1"); //此行必须
	}
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
	function SetFullScreen() {
		document.getElementById("PDFCtrl1").FullScreen = !document
				.getElementById("PDFCtrl1").FullScreen;
	}
</script>
<form id="form1"><input name="token" type="hidden"
	value="<%=request.getAttribute("filetoken")%>" /></form>

<po:PDFCtrl id="PDFCtrl1" />
</body>
</html>
