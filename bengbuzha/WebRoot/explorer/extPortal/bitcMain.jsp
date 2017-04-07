<!DOCTYPE HTML>
<%@page import="org.apache.commons.lang.StringUtils"%>
<%@page import="com.bitc.jk.xmis.util.DateUtil"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="javax.servlet.ServletContext"%>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" />
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<%
String path = request.getContextPath();
EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
if(emp!=null){
  out.print("<script type=\"text/javascript\">");
  out.print("var __menus="+emp.buildUserMenuJsonStr()+";");
	  String lastDate =(String)session.getAttribute(WebKeys.Session_Attribute_LAST_LOGIN_TIME);
	  int logCount = (Integer)session.getAttribute(WebKeys.Session_Attribute_LOGIN_COUNT);
	  if(StringUtils.isEmpty(lastDate)){
		  lastDate="";
	  }
	  String nowDate = DateUtil.formatDateTime(DateUtil.getCurrentDateTime());
	  out.print("var lastDate='"+lastDate+"';");
	  out.print("var logCount='"+logCount+"';");
	  out.print("var nowDate='"+nowDate+"';");
  out.print("</script>");
}

ServletContext servletContext = request.getSession()
.getServletContext();
String timer_task_interval = (String) servletContext
.getAttribute("timer_task_interval");
%>
<link rel="icon" href="<%=path%>/favicon.ico" type="image/x-icon" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all-notheme.css" />
<link rel="stylesheet" type="text/css" title="blue" href="<%=path%>/resources/css/xtheme-blue.css" />
<link rel="stylesheet" type="text/css" title="gray" href="<%=path%>/resources/css/xtheme-gray.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-custom.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/grid-examples.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/calendarStyle.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/styleCIS.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/Portal.css" />

<script type="text/javascript" src="<%=path%>/dynamic.jsp?<%=new Date().getTime() %>">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-base.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-all-debug.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-lang-zh_CN.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-extra.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/styleswitcher.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/Portal.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/PortalColumn.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/Portlet.js">
</script>


<script type="text/javascript" src="<%=path%>/resources/js/StatusBar.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/TabContextMenu.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ActionListColumn.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/TodoList.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/MessageInfoList.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/Attention.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/ContractState.js">
</script>
<script type="text/javascript" src="<%=path%>/applymanage/applyMeetingFormTipsGrid.js"/>
 
<script type="text/javascript" src="<%=path%>/resources/js/PaymentPromptList.js">
</script>
<script type="text/javascript" src="<%=path%>/resources/js/Draft.js">
</script>

<style type="text/css">
  body .x-panel { margin-bottom: 0px; } 
  a.loginfo:VISITED { text-decoration: none; color: white; }
  a.loginfo:hover { text-decoration: none; color: white; }
  a.loginfo:link { text-decoration: none; color: white; }
  a.loginfo:active { text-decoration: none; color: white; }
  a {text-decoration:none !important;}
</style>
<script type="text/javascript" src="<%=path%>/extPortal/bitcMain.js">
</script>

<script type="text/javascript">
   	var empId = "<%=emp.getUserID()%>";
   	var timer_task_interval ="<%=timer_task_interval%>";
</script>
<title>
  蚌埠闸工程管理处  综合信息管理系统
</title>
</head>
<body>
</body>
</html>

