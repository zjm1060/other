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
    
    <script type="text/javascript" src="<%=path %>/security/securityExtTest.js"></script>
  </head>
  <body id="docbody">
  </body>
</html>