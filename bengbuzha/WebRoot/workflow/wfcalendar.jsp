<%@page contentType="text/html; charset=utf-8"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
      .holiday { color:red;}
    </style>
    <script type="text/javascript" src="<%=path%>/workflow/js/wfcalendar.js">
    </script>
    <title>
      北京中水科技 X-MIS
    </title>
       	<script type="text/javascript">
        	var isAdminUser = com.bitc.xmis.existRole(__emp.roles, 2);//系统管理员
		</script>
  </head>
  <body>
  </body>
</html>

