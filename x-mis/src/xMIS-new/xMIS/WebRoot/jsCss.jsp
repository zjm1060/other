<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.util.PropertyFilter"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%
	String path = request.getContextPath();
	if("/".equals(path)){
		path="";
	}
%>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" />
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<link rel="icon" href="<%=path%>/favicon.ico" type="image/x-icon" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all-notheme.css" />
<link rel="stylesheet" type="text/css" title="blue" href="<%=path%>/resources/css/xtheme-blue.css" />
<link rel="stylesheet" type="text/css" title="gray" href="<%=path%>/resources/css/xtheme-gray.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-custom.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/calendarStyle.css"/>
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/RowEditor.css" />
<link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/styleCIS.css" />

<script type="text/javascript" src="<%=path%>/dynamic.jsp"></script>
<script type="text/javascript" src="<%=path%>/resources/js/DBShcma.jsp"></script>
<script type="text/javascript" src="<%=path%>/resources/js/WebKeys.jsp"></script>
<script type="text/javascript" src="<%=path%>/resources/js/bitc.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/common.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-base.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-all-debug.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-lang-zh_CN.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/SummaryFormat.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/ext-extra.js"></script>
<script type="text/javascript" src="<%=path%>/resources/js/styleswitcher.js"></script>