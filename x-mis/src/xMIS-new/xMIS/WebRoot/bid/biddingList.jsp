<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<!DOCTYPE HTML>
<html>
  <head>
   	<%@include file="../jsCss.jsp" %>
   	<link rel="stylesheet" type="text/css" href="<%=path %>/resources/css/LockingGridView.css" />
   	
   	<script type="text/javascript" src="<%=path %>/resources/js/LockingGridView.js"></script>
   	<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
   	<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
   	<script type="text/javascript" src="<%=path%>/resources/js/ColumnHeaderGroup.js"></script>
   	<script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js"></script>
   	<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    <script type="text/javascript" src="<%=path%>/bid/BidCompanyWindow.js"></script>
    <script type="text/javascript" src="<%=path%>/bid/biddingList.js"></script>    
    <script type="text/javascript" src="<%=path%>/resources/echarts/echarts.js"></script>
    <script type="text/javascript">
        // 路径配置
        require.config({
            paths: {
                echarts: '${pageContext.request.contextPath}/resources/echarts'
            }
        });
    </script>
    <title>
    </title>
    <style type="text/css">
    	.detaildiv{
    		margin: 5px auto;
    	}
    </style>
    
  </head>
  <body>
  </body>
</html>

