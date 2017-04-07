<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE HTML>

<html>
  <head>
    <title></title>
    <%@include file="../jsCss.jsp"%>
    <style type="text/css">
    	.x-pivotgrid .x-grid3-row-headers table td {
    		height: 25px; !important
		}
		.x-grid3-cell-inner,.x-grid3-hd-inner {
			overflow: hidden;
			-o-text-overflow: ellipsis;
			text-overflow: ellipsis;
			padding: 0px 0px 0px 0px; !important
	}
	.x-grid3-cell-first .x-grid3-cell-inner {
		padding-left: 0px;!important
	}
    </style>
    <script type="text/javascript" src="<%=path %>/resources/js/tdgi2.borderLayout.js"></script>
    <script type="text/javascript" src="<%=path %>/developmentProgress/PersonWeekAccounting.js"></script>
    <script type="text/javascript" src="<%=path %>/developmentProgress/PersonMonthAccounting.js"></script>
    <script type="text/javascript" src="<%=path %>/developmentProgress/accounting.js"></script>
  </head>
  <body>
  </body>
</html>