<%@ page language="java"
	import="java.util.*,java.text.SimpleDateFormat,com.bitc.jk.xmis.util.Type,com.bitc.jk.xmis.util.WebUtil,com.bitc.jk.xmis.util.WebKeys,com.zhuozhengsoft.pageoffice.*"
	pageEncoding="gb2312"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	//******************************卓正PageOffice组件的使用*******************************
	PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
	poCtrl1.setServerPage(request.getContextPath()+"/poserver.zz"); //此行必须

	String directPath = (String) getServletContext().getAttribute(
			WebKeys.Session_Application_Cfg_AttachedPath) ;// 在硬盘中的路径
	String path = request.getSession().getServletContext().getRealPath("officialdocument");
	WordDocument worddoc = new WordDocument();
	//先在要插入word文件的位置手动插入书签,书签必须以“PO_”为前缀
	//给DataRegion赋值,值的形式为："[word]word文件路径[/word]、[excel]excel文件路径[/excel]、[image]图片路径[/image]"
	
	
	//表格数据
	String formId = Type.getString(request.getParameter("form_id"));
		
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		 String appTime ="";
		 String deptTime="";
		 String specTime ="";
		 String direTime = "";
		 String sendYear ="";
		 String sendMonth ="";
		 String sendDay ="";
		 String appYear ="";
		 String appMonth ="";
		 String appDay ="";
		if(Type.getDateTime(request.getParameter("app_datetime")) !=null){
		  appTime = sdf.format(Type.getDateTime(request.getParameter("app_datetime")));
		  appYear = appTime.substring(0,4);
		  appMonth = ((Type.getDateTime(request.getParameter("app_datetime"))).getMonth()+1)+"";
		  appDay = (Type.getDateTime(request.getParameter("app_datetime"))).getDate()+"";
		}
		if(Type.getDateTime(request.getParameter("app_dept_approved_datetime"))!=null){
	      deptTime = sdf.format(Type.getDateTime(request.getParameter("app_dept_approved_datetime")));
		}
		if(Type.getDateTime(request.getParameter("app_spec_approved_datetime"))!=null){
		 specTime = sdf.format(Type.getDateTime(request.getParameter("app_spec_approved_datetime")));
		}
		if(Type.getDateTime(request.getParameter("app_direc_approved_datetime"))!=null){
		 direTime = sdf.format(Type.getDateTime(request.getParameter("app_direc_approved_datetime")));
		 sendYear = direTime.substring(0,4);
		 sendMonth = ((Type.getDateTime(request.getParameter("app_direc_approved_datetime"))).getMonth()+1)+"";
		 sendDay = (Type.getDateTime(request.getParameter("app_direc_approved_datetime"))).getDate()+"";
		}
	DataTag deptTag = worddoc.openDataTag("[会签领导]");
	deptTag.setValue(Type.getString(request.getParameter("HQ_name")));
	
	DataTag spcTag = worddoc.openDataTag("[分管领导]");
	spcTag.setValue(Type.getString(request.getParameter("app_spec_leader_name"))+ "           "+specTime);
	
	DataTag dirTag = worddoc.openDataTag("[主管领导]");
	dirTag.setValue(Type.getString(request.getParameter("app_direc_leader_name"))+"           "+direTime);
	
	DataTag appUser = worddoc.openDataTag("[申请人]");
	appUser.setValue(Type.getString(request.getParameter("app_user_name")));
	
	DataTag summary = worddoc.openDataTag("[摘要]");
	summary.setValue(Type.getString(request.getParameter("summary")));
	
	DataTag attFile = worddoc.openDataTag("[附件1]");
	attFile.setValue(Type.getString(request.getParameter("attFile")));
	
	DataTag external_name = worddoc.openDataTag("[发送机关]");
	external_name.setValue(Type.getString(request.getParameter("external_name")));
	
	
	//DataTag title = worddoc.openDataTag("[标题]");
	//title.setValue(Type.getString(request.getParameter("title")));
	
	DataTag appyear = worddoc.openDataTag("[yyyy]");
	appyear.setValue(appYear);
	
	DataTag appmonth = worddoc.openDataTag("[mm]");
	appmonth.setValue(appMonth);
	
	DataTag appday = worddoc.openDataTag("[dd]");
	appday.setValue(appDay);
	
	DataTag sendyear = worddoc.openDataTag("[年]");
	sendyear.setValue(sendYear);
	
	DataTag sendmonth = worddoc.openDataTag("[月]");
	sendmonth.setValue(sendMonth);
	
	DataTag sendday = worddoc.openDataTag("[日]");
	sendday.setValue(sendDay);
	//正文
	DataRegion data1 = worddoc.openDataRegion("PO_ZW");
	data1.setValue("[word]"+directPath+"正文"+formId+".doc[/word]");
	poCtrl1.setWriter(worddoc);
	poCtrl1.setCaption("预览拟稿");

	//隐藏菜单栏
	poCtrl1.setMenubar(false);
	//隐藏自定义工具栏
	poCtrl1.setCustomToolbar(false);
	poCtrl1.setSaveFilePage("SaveShowFile.jsp");

	poCtrl1.webOpen(path+"\\document.doc", OpenModeType.docNormalEdit, WebUtil.getCurrentEmployee().getUserName());
	poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须
	
	
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<title>后台编程插入Word文件到数据区域</title>
	</head>
	<body>
		<form id="form1">
			<div style="width: auto; height: 700px;">
				<!--**************   PageOffice 客户端代码开始    ************************-->
				<po:PageOfficeCtrl id="PageOfficeCtrl1">
				</po:PageOfficeCtrl>
				<!--**************   PageOffice 客户端代码结束    ************************-->
			</div>
		</form>
	</body>
</html>