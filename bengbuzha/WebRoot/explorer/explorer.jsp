<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%
	String path = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="pragma" content="no-cache">
<meta http-equiv="cache-control" content="no-cache">
<meta http-equiv="expires" content="0">
<title>ExplorerView</title>
<link rel="stylesheet" type="text/css"
	href="<%=path%>/explorer/explorerview.css" />
<script type="text/javascript"
	src="<%=path%>/explorer/Ext.ux.grid.ExplorerView.js"></script>
<script type="text/javascript"
	src="<%=path%>/explorer/Ext.ux.grid.GroupingExplorerView.js"></script>
<script type="text/javascript" src="<%=path%>/explorer/explorerview.js"></script>
<style type="text/css">
.x-grid-group-body-floatcontainer:after {
	content: ".";
	display: block;
	height: 0;
	font-size: 0;
	clear: both;
	visibility: hidden;
}
.x-grid-group-body-floatcontainer { display: inline-block; }
/* Hides from IE Mac \*/
* html .x-grid-group-body-floatcontainer { height: 1%; }
.x-grid-group-body-floatcontainer { display: block; }
/* End Hack */ 

.ux-explorerview-item {
	float: left; display: block; border: none;
}


body .x-panel {
	margin-bottom:20px;
}
.icon-grid {
	background-image:url(../shared/icons/fam/grid.png) !important;
}


/* Large Icons */
.ux-explorerview-large-item { width: 100px; height: 100px; margin: 5px 0 0 5px; -moz-user-select: none; }
.ux-explorerview-large-item .ux-explorerview-icon { width: 100px; height: 80px; }
.ux-explorerview-large-item .ux-explorerview-icon td { padding-top: 8px; }
.ux-explorerview-large-item .ux-explorerview-text { text-align: center; padding: 0 5px; white-space: nowrap; overflow: hidden; }

/* Medium Icons */
.ux-explorerview-medium-item { width: 80px; height: 80px; margin: 5px 0 0 5px; -moz-user-select: none; }
.ux-explorerview-medium-item .ux-explorerview-icon { width: 80px; height: 60px; }
.ux-explorerview-medium-item .ux-explorerview-icon td { padding-top: 8px; }
.ux-explorerview-medium-item .ux-explorerview-text { text-align: center; padding: 0 5px; white-space: nowrap; overflow: hidden; }

/* Small Icons */
.ux-explorerview-small-item { width: 180px; height: 18px; margin: 5px 0 0 5px; -moz-user-select: none; }
.ux-explorerview-small-item .ux-explorerview-icon { float: left; width: 20px; height: 18px; }
.ux-explorerview-small-item .ux-explorerview-text { float: left; width: 150px; padding-top: 2px; white-space: nowrap; overflow: hidden; }

/* Tiles */
.ux-explorerview-tiles-item { width: 180px; height: 48px; margin: 5px 0 0 5px; -moz-user-select: none; }
.ux-explorerview-tiles-item .ux-explorerview-icon { float: left; width: 50px; height: 50px; }
.ux-explorerview-tiles-item .ux-explorerview-text { float: left; width: 120px; padding-top: 8px; white-space: nowrap; overflow: hidden; }
.ux-explorerview-tiles-item .ux-explorerview-text span { color: #777; }

</style>
</head>
<body>
<div id="explorerDIV1"></div>
<div id="explorerDIV"></div>
</body>
</html>