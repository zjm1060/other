<%@page contentType="text/html; charset=utf-8"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
%>
<html>
  <head>
    <meta http-equiv="pragma" content="no-cache">
    <meta http-equiv="cache-control" content="no-cache">
    <meta http-equiv="expires" content="0">
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all.css" />
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-custom.css" />
    <script type="text/javascript">
      var __cxtPath="<%=path%>";
      var __token="<%=request.getAttribute("reportToken")%>"
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-base.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-all-debug.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-lang-zh_CN.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-extra.js">
    </script>
    <script type="text/javascript" src="<%=path%>/report/js/gridReport.js">
    </script>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>
