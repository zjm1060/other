<%@page import="com.bitc.jk.xmis.model.Node"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@page import="com.bitc.jk.xmis.dao.impl.OrganizationDAOImpl"%>
<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html>
<%
	OrganizationDAOImpl dao = SpringInit.getBean("organizationDAOImpl",
			OrganizationDAOImpl.class);
	Node root = dao.getNodes();
%>
<html>
<head>
<title></title>
</head>
<body>
<table border="1">
	<thead>
		<tr>
			<th>角色ID</th>
			<th>角色名称</th>
			<th>部门ID</th>
			<th>部门名称</th>
			<th>人员ID</th>
			<th>人员名称</th>
		</tr>
	</thead>
	<tbody>
		<%for(int i=0;i<root.getChildren().size();i++){
			Node role=root.getChildren().get(i);
			%>
				<tr>
					<td rowspan="<%=role.getSize()%>"><%=role.getId() %></td>
					<td rowspan="<%=role.getSize()%>"><%=role.getText() %></td> 
			<%
			for(int j=0;j<role.getChildren().size();j++){
				Node dept=role.getChildren().get(j);
				if(j!=0){
					%>
					<tr>
					<%
				}
				%>
					<td rowspan="<%=dept.getSize()%>"><%=dept.getId() %></td>
					<td rowspan="<%=dept.getSize()%>"><%=dept.getText() %></td> 
				<%
				for(int k=0;k<dept.getChildren().size();k++){
					Node emp=dept.getChildren().get(k);
					if(k!=0){
						%>
						<tr>
						<%
					}
					%>
					<td><%=emp.getId() %></td>
					<td><%=emp.getText() %></td>
					</tr> 
					<%
				}
			}
		} 
		%>
	</tbody>
</table>
</body>
</html>

