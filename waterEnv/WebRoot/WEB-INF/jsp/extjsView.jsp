<%@ page import="bitc.sz.wzEnv.util.WebUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: WU-HOME
  Date: 2016/7/2 0002
  Time: 21:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
    <%
        String path= WebUtil.getContextPath(request);
    %>
    <script type="text/javascript">
        var __path='<%=path%>';
    </script>
    <title></title>
    <%@include file="extjsJsCss.jsp" %>
    <script type="text/javascript" src="${pageContext.request.contextPath}/resources/echarts/echarts-all.js"></script>
</head>
<body>
<script>
    Ext.onReady(function(){
        new Ext.container.Viewport({
            layout: 'border',
            items: [Ext.create('${param["viewClass"]}', Ext.apply({
                region:'center',
                border:false
            }, ${param["config"]}))]
        });
    });

</script>
</body>
</html>
