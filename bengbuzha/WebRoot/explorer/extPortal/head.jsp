<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<!DOCTYPE HTML>
<%
	String path = request.getContextPath();
	if("/".equals(path)){
		path="";
	}
%>

<html>
  <head>
    
    <title>My JSP 'head.jsp' starting page</title>
    
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
		<table width="100%">
			<tr>
				<td rowspan="2">
					<img src="<%=path %>/resources/images/top-title.jpg">
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
									<a href=<%=path+"/logout.do"%> > 注销</a>
									
				</TH>
			</tr>
		</table>
</body>
</html>
