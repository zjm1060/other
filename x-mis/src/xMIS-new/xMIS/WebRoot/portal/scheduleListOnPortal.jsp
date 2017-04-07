<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE HTML>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
	
//	System.out.println(type);
%>

<html>
	<head>
		<base href="<%=basePath%>">

		<title>My JSP 'taskListOnPortal.jsp' starting page</title>

		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">
		<link rel="stylesheet" type="text/css"
			href="<%=path %>/resources/css/styleCIS.css">
		<script type="text/javascript" src="../resources/js/common.js"></script>

	</head>

	<body>
		<div id=activedTaskDiv class=taskDivList >
		<TABLE class=grid id=activedTaskList cellSpacing=0 cellPadding=0 border=0 width=90%>
			<TR class=odd>
				<td>
					<a href=#>[会议]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期设计联络会</a>
				</td>
				<td>
					<a href=#>06/08/2010--06/10/2010</a>
				</td>
			</tr>
			<TR class=even>
				<td>
					<a href=#>[出差]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期</a>
				</td>
				<td>
					<a href=#>05/08/2010--05/10/2010</a>
				</td>
			</tr>
			<TR class=odd>
				<td>
					<a href=#>[外出]</a>
				</td>
				<td>
					<a href=#>年假</a>
				</td>
				<td>
					<a href=#>05/20/2010--05/22/2010</a>
				</td>
			</tr>
		</table>
		</div>
		<p><a href=#>more...</a></p>

	</body>
</html>

