<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
	
//	System.out.println(type);
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
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
			href="../resources/css/styleCIS.css">
		<script type="text/javascript" src="../resources/js/common.js"></script>

	</head>

	<body>
		<div id=activedTaskDiv class=taskDivList >
		<TABLE class=grid id=activedTaskList cellSpacing=0 cellPadding=0 border=0 width=90%>
			<TR class=odd>
				<td>
					User1
				</td>
				<td>
					[会议]<a href=#>三峡水电站二期研讨</a>
				</td>
				<td>
					05/18/2010--05/20/2010
				</td>
			</tr>
			<TR class=even>
				<td>
					User2
				</td>
				<td>
					[出差]<a href=#>三峡水电站二期</a>
				</td>
				<td>
					05/17/2010--05/30/2010
				</td>
			</tr>
			<TR class=odd>
				<td>
					User3
				</td>
				<td>
					[外出]<a href=#>年假</a>
				</td>
				<td>
					05/18/2010--05/20/2010
				</td>
			</tr>
		</table>
		</div>

	</body>
</html>

