<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>


<%
	String type = null;
//	type = (String) request.getParameter("type");
	type = request
			.getParameter(WebKeys.Request_Attribute_Name_Type) == null ? (String) request
			.getAttribute(WebKeys.Request_Attribute_Name_Type)
			: request.getParameter(WebKeys.Request_Attribute_Name_Type);

	String currentDate = new java.sql.Date(System.currentTimeMillis())
			.toString();

	String currentDay = new String();
	currentDay = "";

	Calendar c = Calendar.getInstance();
	c.setTime(new Date(System.currentTimeMillis()));
	int dayOfWeek = c.get(Calendar.DAY_OF_WEEK);
	int d = c.get(Calendar.DATE);
	
	switch (dayOfWeek) {
	case 1:
		currentDay = "星期日";
		break;
	case 2:
		currentDay = "星期一";
		break;
	case 3:
		currentDay = "星期二";
		break;
	case 4:
		currentDay = "星期三";
		break;
	case 5:
		currentDay = "星期四";
		break;
	case 6:
		currentDay = "星期五";
		break;
	case 7:
		currentDay = "星期六";
		break;
	}

	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ request.getContextPath() + "/";
	
%>

<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<LINK href="/xMIS/resources/css/styleTab.css" type=text/css
			rel=stylesheet>
		<LINK href="/xMIS/resources/css/styleCIS.css" type=text/css
			rel=stylesheet>

		<title>北京中水科技 监控事业部 综合信息管理系统</title>
	</head>
	<body>
		<table width="100%">
			<tr>
				<td rowspan="2">
					<img src="/xMIS/resources/images/top-title.jpg">
				</td>
				<TH class=large noWrap valign="bottom" colSpan=2>
					<H2>
						<font color=blue><%=((EmployeeObj) session
									.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
									.getDivisionName()%></font>
					</H2>
					<!--iframe src="http://m.weather.com.cn/m/pn4/weather.htm "
						width="160" height="20" marginwidth="0" marginheight="0"
						hspace="0" vspace="0" frameborder="0" scrolling="no" allowTransparency=true></iframe-->

				</TH>

			</tr>
			<tr>
				<TH class=large noWrap>
					<font color="#0000FF">欢迎 <%=((EmployeeObj) session
									.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
									.getUserName()%></font>
									
				</TH>
				<TH class=large noWrap>
									<a href=<%=basePath+"logout.do"%> > 注销</a>
									
				</TH>
			</tr>
		</table>

		<jsp:include page="<%=WebKeys.Include_JSP_File_mainmenu %>" />
		<br>
		<table width="100%" height="88%">
			<tr>
				<td width='100%' valign="top">
					<%
					if (type == null) {
					%>
					<jsp:include page="<%=WebKeys.Include_JSP_File_xMisPortal %>" />
					<%
					} else {
					%>
					<jsp:include page="<%=WebKeys.Include_JSP_File_subMain %>">
						<jsp:param name="type" value="<%=type%>" />
					</jsp:include>
					<%
					}
					%>
				</td>
			</tr>
		</table>
	</body>
</html>
<DIV id=calendarDiv style="VISIBILITY: hidden; POSITION: absolute; BACKGROUND-COLOR: white; layer-background-color: white; border:0px; z-index:9999;"></DIV>



