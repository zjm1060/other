<%@ page import="com.bitc.jk.xmis.util.WebUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2017/1/19
  Time: 9:04
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<%
  if ("处领导".equals(WebUtil.getCurrentEmployee().getDepartmentName()) || "工程科".equals(WebUtil.getCurrentEmployee().getDepartmentName())) {
%>
<head>
  <title></title>
  <script type="text/javascript" src="jquery_ui/jquery-1.10.2.min.js"></script>
  <link rel="stylesheet" type="text/css" href="bootstrap/css/bootstrap.min.css">
  <script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
  <script>
    var __path = "<%=request.getContextPath()%>";
    function loadDiv(url){
        window.open(__path+url,"_self");
    }
    $(function(){
      window.open(__path+'/gczl/shangxiashuiweiliuliang.jsp',"_self");
    })
  </script>
</head>
<body>
<div class="container">
  <div class="row" style="margin-top: 20px;">
    <div class="col-md-12">
        <nav class="navbar navbar-default" role="navigation">
          <div class="container-fluid">
            <div class="navbar-header">
              <a class="navbar-brand" href="#"><strong>蚌埠闸管理系统</strong></a>
            </div>
            <div>
              <ul class="nav navbar-nav">
                <li class="active"><a href="#"onclick="loadDiv('/gczl/shangxiashuiweiliuliang.jsp')">水位流量折线图</a></li>
                <li><a href="#" onclick="loadDiv('/gczl/doDayReport.jsp')">闸门控制系统报表</a></li>
                <li><a href="#" onclick="loadDiv('/gczl/zmDayReport.jsp')">闸门控制系统日报表</a></li>
              </ul>
            </div>
          </div>
        </nav>
    </div>
  </div>
</div>
</body>
<%
} else {
%>
<div class="container row">
  <div style="margin-top: 300px;text-align: center" class="col-md-6 col-md-offset-3"><h1>该功能仅对处领导与工程科公开！</h1></div>
</div>
</body>
<%
  }
%>
</html>
