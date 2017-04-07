<%@ page language="java"
	import="java.util.*,com.zhuozhengsoft.pageoffice.*,com.zhuozhengsoft.pageoffice.wordwriter.*"
	pageEncoding="utf-8"%>
<%@ taglib uri="http://java.pageoffice.cn" prefix="po"%>
<%
	String formId= request.getParameter("form_id");
	String attId = request.getParameter("attachmentTypeID");
	%> 

<%
//******************************卓正PageOffice组件的使用*******************************
	PageOfficeCtrl poCtrl1 = new PageOfficeCtrl(request);
	poCtrl1.setServerPage(request.getContextPath()+"/poserver.zz"); //此行必须
	poCtrl1.setTitlebar(false); //隐藏标题栏
	//隐藏菜单栏
	poCtrl1.setMenubar(false);
	poCtrl1.setCustomToolbar(false);//隐藏自定义工具栏
	//隐藏工具栏
	poCtrl1.setCustomToolbar(false);
	//设置保存页面
	poCtrl1.setSaveFilePage("SaveNewFile.jsp?attachmentTypeID="+attId+"&REFID="+formId);
	//新建Word文件，webCreateNew方法中的两个参数分别指代“操作人”和“新建Word文档的版本号”
	poCtrl1.webCreateNew("",DocumentVersion.Word2003);
	poCtrl1.setTagId("PageOfficeCtrl1"); //此行必须	
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<title></title>
		<script type="text/javascript">
		    </script>
	</head>
	<body>
		<form id="form2" action="CreateWord.aspx">
			<div id="header">
				<div style="float: left; margin-left: 20px;">
				</div>
			</div>
			<div id="content">
				<div id="textcontent" style="width: 1000px; height: 800px;">
					<div class="flow4">
						<span style="width: 100px;"> &nbsp; </span>
					</div>
					<div>
						<input  name="FileSubject"  id="FileSubject" type="hidden" value="" />
					</div>
					<div>
						&nbsp;
					</div>
					<po:PageOfficeCtrl id="PageOfficeCtrl1" />
				</div>
			</div>
			<div id="footer">
				<hr width="1000px;" />
			</div>
		</form>
	</body>
</html>
