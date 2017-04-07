<%@page import="java.util.ArrayList"%>
<%@page import="org.wltea.expression.datameta.Variable"%>
<%@page import="org.wltea.expression.ExpressionEvaluator"%>
<%@page import="java.util.List"%>
<%@page pageEncoding="UTF-8" contentType="text/html; charset=UTF-8"%>
<!DOCTYPE HTML>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title></title>
</head>
<body>
<%
	List<Variable> variables = new ArrayList<Variable>();
	variables.add(Variable.createVariable("roleId", 16));
	//执行表达式
	List<String> result = (List<String>) ExpressionEvaluator.evaluate(
			"$deptRole(roleId,1)", variables);
	out.print(result.toString());
%>

</body>
</html>
