<%@ page language="java"
	import="java.util.*,java.text.SimpleDateFormat,com.bitc.jk.xmis.util.Type,com.zhuozhengsoft.pageoffice.FileMakerCtrl,com.bitc.jk.xmis.util.WebUtil,com.bitc.jk.xmis.util.WebKeys,com.zhuozhengsoft.pageoffice.*,com.zhuozhengsoft.pageoffice.wordwriter.*;"
	pageEncoding="gb2312"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	//******************************卓正PageOffice组件的使用*******************************
	FileMakerCtrl fmCtrl = new FileMakerCtrl(request);
	fmCtrl.setServerPage(request.getContextPath() + "/poserver.zz");

	String formId = Type.getString(request.getParameter("form_id"));
	String directPath = (String) getServletContext().getAttribute(
			WebKeys.Session_Application_Cfg_AttachedPath);// 在硬盘中的路径
	String basepath = request.getSession().getServletContext()
			.getRealPath("officialdocument");
	int document_type_id = Type.GetInt(request
			.getParameter("document_type_id"));
	WordDocument worddoc = new WordDocument();
	worddoc.setDisableWindowRightClick(true);
	//先在要插入word文件的位置手动插入书签,书签必须以“PO_”为前缀
	//给DataRegion赋值,值的形式为："[word]word文件路径[/word]、[excel]excel文件路径[/excel]、[image]图片路径[/image]"

	//表格数据

	SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	String sendYear = "";
	if (Type.getDateTime(request.getParameter("approved_datetime")) != null) {
		String direTime = sdf.format(Type.getDateTime(request
				.getParameter("approved_datetime")));
		sendYear = direTime.substring(0, 4);
	}
	
	String ext_name = Type.getString(request.getParameter("external_name"));
	if(!"".equals(ext_name)){
		DataRegion external_name = worddoc.openDataRegion("PO_ext_name");
		external_name.setValue(ext_name+"：");
	}

	DataRegion year = worddoc.openDataRegion("PO_Year");
	year.setValue(sendYear);

	DataTag title = worddoc.openDataTag("[标题]");
	title.setValue(Type.getString(request.getParameter("title")));
	String file_flag = Type
			.getString(request.getParameter("file_flag")).trim();
	DataRegion flag = worddoc.openDataRegion("PO_FLAG");
	flag.setValue(file_flag);

	String app_direc_leader_name = Type.getString(request
			.getParameter("app_direc_leader_name"));
	DataRegion leader_name = worddoc.openDataRegion("PO_leader_name");
	leader_name.setValue(app_direc_leader_name);
	DataTag attFile = worddoc.openDataTag("[附件]");
	if ("".equals(Type.getString(request.getParameter("attFile")))) {
		attFile.setValue("");
	} else {
		attFile.setValue("附件："
				+ Type.getString(request.getParameter("attFile")));
	}
	//正文
	DataRegion data1 = worddoc.openDataRegion("PO_ZW");
	data1.setValue("[word]" + directPath + "正文" + formId
			+ ".doc[/word]");

	String path = request.getSession().getServletContext()
			.getRealPath("officialdocument");
	fmCtrl.setSaveFilePage("SaveFile.jsp?formId=" + formId);//如要保存文件，此行必须
	fmCtrl.setWriter(worddoc);
	fmCtrl.setJsFunction_OnProgressComplete("OnProgressComplete()");
	if ("蚌埠闸处".equals(file_flag)) {
		if (document_type_id == 1) {
			fmCtrl.fillDocumentAs(path + "\\xingwen.doc",
					DocumentOpenType.Word, "行文" + formId + ".doc");
		} else if (document_type_id == 3) {
			fmCtrl.fillDocumentAs(path + "\\xingwen3.doc",
					DocumentOpenType.Word, "行文" + formId + ".doc");
		}
	} else if ("蚌埠闸党".equals(file_flag)) {
		if (document_type_id == 1) {
			fmCtrl.fillDocumentAs(path + "\\xingwen4.doc",
					DocumentOpenType.Word, "行文" + formId + ".doc");
		} else if (document_type_id == 3) {
			fmCtrl.fillDocumentAs(path + "\\xingwen2.doc",
					DocumentOpenType.Word, "行文" + formId + ".doc");
		}
	}

	fmCtrl.setTagId("FileMakerCtrl1"); //此行必须
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>合成行文</title>
</head>
<script language="javascript" type="text/javascript">
	function OnProgressComplete() {
		alert("行文生成成功！");
		window.close();
	}
</script>
<body>
<form id="form1">
<div style="width: auto; height: 700px;"><!--**************   PageOffice 客户端代码开始    ************************-->
<po:FileMakerCtrl id="FileMakerCtrl1" /> <!--**************   PageOffice 客户端代码结束    ************************-->
</div>
</form>
<h3>正在合成行文，请稍等。。。</h3>
</body>
</html>