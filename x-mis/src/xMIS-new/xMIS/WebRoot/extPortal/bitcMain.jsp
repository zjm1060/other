<!DOCTYPE HTML>
<%@page import="org.apache.commons.lang.StringUtils"%>
<%@page import="com.bitc.jk.xmis.util.DateUtil"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@ page import="com.bitc.jk.xmis.dao.SysLogDAO"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>

<html>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" />
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<%
	String path = request.getContextPath();
	EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
			WebKeys.Session_Attribute_Name_UserDNA);
	if (emp != null) {
		out.print("<script type=\"text/javascript\">");
		out.print("var __menus=" + emp.buildUserMenuJsonStr() + ";");
		String lastDate = (String) session
				.getAttribute(WebKeys.Session_Attribute_LAST_LOGIN_TIME);
		int logCount = (Integer) session
				.getAttribute(WebKeys.Session_Attribute_LOGIN_COUNT);
		if (StringUtils.isEmpty(lastDate)) {
			lastDate = "";
		}
		String nowDate = DateUtil.formatDateTime(DateUtil
				.getCurrentDateTime());
		out.print("var lastDate='" + lastDate + "';");
		out.print("var logCount='" + logCount + "';");
		out.print("var nowDate='" + nowDate + "';");

		
		SysLogDAO sysLogDAO = (SysLogDAO)SpringInit.getBean("sysLogDAO");
		    
		List todayList = sysLogDAO.getTodayList(emp.getUserID());
		List yesterdayList = sysLogDAO.getYesterdayList(emp.getUserID());

		String todayDate = DateUtil.formatDate(DateUtil.getCurrentDate());
		String todayEarly = "";
		String todayLate = "";

		String yesterdayDate = "";
		String yesterdayEarly = "";
		String yesterdayLate = "";

		if (todayList != null && todayList.size() > 0) {
			Object[] todayObj = (Object[]) todayList.get(0);
			todayDate = Type.getString(todayObj[0]);
			if (!"".equals(Type.getString(todayObj[0]))) {
				todayDate = Type.getString(todayObj[0]);
			}
			if (!"".equals(Type.getString(todayObj[1]))) {
				todayEarly = Type.getString(todayObj[1]);
			}
			if (!"".equals(Type.getString(todayObj[2]))) {
				todayLate = Type.getString(todayObj[2]);
			}
		}
 

		if (yesterdayList != null && yesterdayList.size() > 0) {
			Object[] yesterdayObj = (Object[]) yesterdayList.get(0);
			if (!"".equals(Type.getString(yesterdayObj[0]))) {
				yesterdayDate = Type.getString(yesterdayObj[0]);
			}
			if (!"".equals(Type.getString(yesterdayObj[1]))) {
				yesterdayEarly = Type.getString(yesterdayObj[1]);
			}
			if (!"".equals(Type.getString(yesterdayObj[2]))) {
				yesterdayLate = Type.getString(yesterdayObj[2]);
			}

		}
		

		out.print("var yesterdayDate='" + yesterdayDate + "';");
		out.print("var yesterdayEarly='" + yesterdayEarly + "';");
		out.print("var yesterdayLate='" + yesterdayLate + "';");

		
		out.print("var todayDate='" + todayDate + "';");
		out.print("var todayEarly='" + todayEarly + "';");
		out.print("var todayLate='" + todayLate + "';");

		out.print("</script>");
	}
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

<script type="text/javascript" src="<%=path%>/dynamic.jsp?<%=new Date().getTime()%>">
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
</script>
<title>
  北京中水科技 X-MIS 综合信息管理系统
</title>
</head>
<body>

</body>
</html>

