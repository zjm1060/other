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
			href="/xMIS/resources/css/styleCIS.css">
		<script type="text/javascript" src="/xMIS/resources/js/common.js"></script>

	</head>

	<body>
		<div id=newTaskDiv class=taskDivList >
		<TABLE class=grid id=newTaskList cellSpacing=0 cellPadding=0 border=0 width=90%>
			<TR class=odd>
				<td>
					<a href=#>2010-05-07</a>
				</td>
				<td>
					<a href=#>[项目]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期软件开发计划</a>
				</td>
				<!--  td>
					<a href=#>Fr: User1</a>
				</td-->
				<td width =20px>
					<input type=button class=blue_button_accept_refuse value="接收" />
				</td>
			</tr>
			<TR class=even>
				<td>
					<a href=#>2010-05-07</a>
				</td>
				<td>
					<a href=#>[合同]</a>
				</td>
				<td>
					<a href=#>三峡水电站二期合同变更</a>
				</td>
				<!--  td>
					<a href=#>Fr: User1-User2授权)</a>
				</td-->
				<td width =20px>
					<input type=button class=blue_button_accept_refuse value="接收" />
				</td>
			</tr>
		</table>
		</div>

	</body>
</html>

