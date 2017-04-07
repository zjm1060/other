<%@ page language="java" contentType="text/html;charset=utf-8"%>
<%@ taglib uri="/WEB-INF/struts-html.tld" prefix="html"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<jsp:useBean id="loginForm" scope="session"
	class="com.bitc.jk.xmis.web.form.LoginForm" />
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";

	String errMessage = (String) request
			.getAttribute(WebKeys.Request_Attribute_Name_LoginFailed);
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<base href="<%=basePath%>">

		<title>北京中水科技 监控事业部 综合信息管理系统</title>

		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">
		<LINK href="resources/css/styleCIS.css" type=text/css rel=stylesheet>
		<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

	</head>

	<body>
		<h2>
			<img src="resources/images/iwhrtec_badge_ron_页眉.png">
			<font color=blue size="+2">监控事业部 X-MIS</font>
		</h2>
		<br>
		<table width="70%" class=grid cellSpacing=0 cellPadding=0 border=0>
			<tr class=even>
				<th width="60%">
					<p>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;X-MIS信息管理系统是对监控事业部主营业务的系统而科学的管理和控制，从而提高工作效率、降低管理成本、使事业部的信息得以共享、知识得以固化，最终实现闭环的精确化管理。






					
					<p>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;X-MIS主要包括：项目管理、合同管理、物资管理、会议管理、客户关系管理、缺陷管理、员工管理、产品管理、知识库管理、意见建议十项业务模块和相关报表处理







					
					<p>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;随着事业部业务涉及的业务领域的不断扩展，X-MIS也将引入新的功能模块，从而提高X-MIS的实用性，易用性。后续模块有：绩效考评、人力资源、在线交流、车辆管理、培训管理等。






					
					<p>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;X-MIS设计应尽可能的可组态以适应工作流程的改变，而工作流程应在可操作、可量化、可控的原则下尽可能的简化。






					
				</th>
				<td>
					<div align="center">
						<html:form action="/loginAction" method="POST" focus="loginName">
							<input type=hidden name=uri value="<%= (String)request.getSession().getAttribute(WebKeys.RequestKey_Original_URI)%>" />
							<input type=hidden name=queryStr value="<%=(String)request.getSession().getAttribute(WebKeys.RequestKey_Original_QueryString)%>"/>
							<table>
								<tr>
									<td>
										<font size=-1 color=red> <%=errMessage == null ? "" : errMessage%>
										</font>
									</td>
								<tr>
									<td>
										用户:
										<html:text property="loginName" ></html:text>
									</td>
								</tr>
								<tr>
									<td>
										口令:
										<html:password property="password"></html:password>
									</td>
								</tr>
								<tr>
									<td align="right">
										<input type="submit" name="login" value="登录">
										<input type="reset" name="reset" value="取消">
									</td>
								</tr>
							</table>
						</html:form>

					</div>
				</td>
			</tr>
		</table>
	</body>
</html>
