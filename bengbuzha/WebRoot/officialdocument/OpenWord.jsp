<%@ page language="java"
	import="java.util.*,com.bitc.jk.xmis.util.WebKeys,com.bitc.jk.xmis.util.Type,com.zhuozhengsoft.pageoffice.*,com.zhuozhengsoft.pageoffice.wordwriter.*"
	pageEncoding="gb2312"%>
<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	
%>
<%
	//******************************卓正PageOffice组件的使用*******************************
	String path = request.getContextPath();
	PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
	poCtrl1.setServerPage(request.getContextPath() + "/poserver.zz"); //此行必须
	//隐藏菜单栏
	poCtrl1.setTitlebar(false); //隐藏标题栏
	poCtrl1.setMenubar(false); //隐藏菜单栏
	//if (!(Type.getInteger(request.getParameter("zw")) == 1)) {
		poCtrl1.setCustomToolbar(false);//隐藏自定义工具栏
	//}
	poCtrl1.setOfficeToolbars(true);//隐藏Office工具条
	//
	//poCtrl1.addCustomToolButton("保存","Save()",1);

	//poCtrl1.addCustomToolButton("加盖印章", "InsertSeal()", 2);
	//poCtrl1.addCustomToolButton("签字", "AddHandSign()", 3);
	//poCtrl1.addCustomToolButton("验证印章", "VerifySeal()", 5);
	//poCtrl1.addCustomToolButton("修改密码", "ChangePsw()", 0);
	//poCtrl1.addCustomToolButton("管理印章", "EditHandSign()", 9);
	//设置保存页面
	poCtrl1.setSaveFilePage("SaveFile.jsp");
	//打开Word文件
	String directPath = (String) getServletContext().getAttribute(
			WebKeys.Session_Application_Cfg_AttachedPath);// 在硬盘中的路径
	if (Type.getInteger(request.getParameter("zw")) == 1) {
		poCtrl1.webOpen(
				directPath
						+ "行文"
						+ Type.getString(request
								.getParameter("form_id")) + ".doc",
				OpenModeType.docNormalEdit, WebUtil
						.getCurrentEmployee().getUserName());
	} else {
		poCtrl1.webOpen(
				directPath
						+ "正文"
						+ Type.getString(request
								.getParameter("form_id")) + ".doc",
				OpenModeType.docNormalEdit, WebUtil
						.getCurrentEmployee().getUserName());
	}
	poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title></title>
<link href="../images/csstg.css" rel="stylesheet" type="text/css" />

</head>
<body>
<form id="form2"></form>
<!-- ****************************PageOffice 组件客户端编程************************************* -->
<script type="text/javascript">
	function Save() {
		document.getElementById("PageOfficeCtrl1").WebSave();
	}
	function InsertSeal() {
		try {
			document.getElementById("PageOfficeCtrl1").ZoomSeal.AddSeal();
		} catch (e) {
		}
	}
	function AddHandSign() {
		document.getElementById("PageOfficeCtrl1").ZoomSeal.AddHandSign();
	}
	function VerifySeal() {
		document.getElementById("PageOfficeCtrl1").ZoomSeal.VerifySeal();
	}
	function ChangePsw() {
		document.getElementById("PageOfficeCtrl1").ZoomSeal.ShowSettingsBox();
	}
	function EditHandSign() {
		window.location = "../adminseal.do";
	}
</script>
<!-- ****************************PageOffice 组件客户端编程结束************************************* -->
<po:PageOfficeCtrl id="PageOfficeCtrl1" />
</body>
</html>
