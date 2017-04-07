<%@page import="java.util.*"%>
<%@ page language="java"
	pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<%@page import="com.bitc.jk.xmis.model.security.NodeInfo"%>
<!DOCTYPE HTML>
<html>
<head>
<%
	NodeInfo root = (NodeInfo)request.getAttribute("node");//存放角色信息
%>
		<%@include file="../jsCss.jsp"%>
		<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/styleTab.css" />
	
<style type="text/css">

</style>

<!-- 
 -->
<script type="text/javascript">
window.onload = function(){
	var tb = document.getElementById("tab");
	var tbody = tb.getElementsByTagName("tbody")[0];
	var trs = tbody.getElementsByTagName("tr");
    for (var i = 0; i < trs.length; i++) {
        if (i % 2 == 0) {
            trs[i].style.backgroundColor = "rgb(249, 249, 249)";
        }
    }
};
</script>	
</head>
<body>
	<table  id="tab" align="left" width="80%" border="0" class="tb" cellpadding="0" cellspacing="0" >
   		<tbody>
    	<tr>
   			<th>角色</th>
   			<th>角色说明</th>
   			<th>部门</th>
   			<th>成员</th>
   		</tr>
		<%for(int i=0;i<root.getChildren().size();i++){//角色数目
			NodeInfo role=root.getChildren().get(i);
			%>
				<tr id="tr">
					<td  width="20%" rowspan="<%=role.getSize()%>"><%=role.getId() %></td>
					<td  width="20%" rowspan="<%=role.getSize()%>"><%=role.getText()== null?'-':role.getText() %></td>
			<%
			for(int j=0;j<role.getChildren().size();j++){ //角色下的部门数目
				NodeInfo dept=role.getChildren().get(j);
				if(j!=0){
					%>
					<tr>
					<%
				}
				%>
					<td  width="20%" rowspan="<%=dept.getSize()%>"><%=dept.getId()== null?'-':dept.getId()  %></td>
				<%
				for(int k=0;k<dept.getChildren().size();k++){//部门下的成员数目
					NodeInfo emp=dept.getChildren().get(k);
					if(k!=0){
						%>
						<tr>
						<%
					}
					%>
					<td width="20%"><%=emp.getId() %></td>
					</tr> 
					<%
				}
			}
		} 
		%>
		<tbody>
	</table>
</body>
</html>
