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
		<div id=paymentListDiv class=taskDivList >
		<TABLE class=grid id=paymentList cellSpacing=0 cellPadding=0 border=0 width=90%>
			<TR class=odd>
				<td>
					<a href=#>BITC-CP-20100401-001</a> [收入]
				</td>
				<td>
					<a href=#>三峡水电站二期</a>
				</td>
				<td  align=right>
					<font color=red>￥397,000</font>
				</td>
			</tr>
			<TR class=even>
				<td>
					<a href=#>BITC-CP-20100401-203</a> [支出]
				</td>
				<td>
					<a href=#>三峡水电站二期采购</a>
				</td>
				<td  align=right>
					<font color=blue>￥17,000</font>
				</td>
			</tr>
		</table>
		</div>

	</body>
</html>

