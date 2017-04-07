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
					<a href=#>[项目]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期软件开发计划</a>
				</td>
				<!-- td>
					<a href=#>(执行人 空)</a>
				</td>
				<td>
					<a href=#>已接收</a>
				</td-->
				<td>
					<a href=#>2010-05-08</a>
				</td>
			</tr>
			<TR class=even>
				<td>
					<a href=#>[合同]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期合同变更</a>
				</td>
				<!--td>
					<a href=#>(执行人 空)</a>
				</td>
				<td>
					<a href=#>已接收</a>
				</td-->
				<td>
					<a href=#>2010-05-08</a>
				</td>
			</tr>
		</table>
		</div>

	</body>
</html>

