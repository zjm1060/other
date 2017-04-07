<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
ContractObj con = (ContractObj) request
.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
WebKeys.Session_Attribute_Name_UserDNA);
if(emp!=null){
  out.print("<script type=\"text/javascript\">");
  out.print("var __menus="+emp.buildUserMenuJsonStr()+";");
  out.print("</script>");
}
%>
<html>
  <head>
   <title>
      X-MIS协同办公系统
    </title>
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" />
    <meta http-equiv="pragma" content="no-cache">
    <meta http-equiv="cache-control" content="no-cache">
    <meta http-equiv="expires" content="0">

   

    <!-- CSS -->
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-all.css" />
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/ext-custom.css" />
    <link rel="stylesheet" type="text/css" href="<%=path %>/resources/css/RowEditor.css" />
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/calendarStyle.css"/>
    <link rel="stylesheet" type="text/css" href="<%=path%>/resources/css/styleCIS.css"/>
    <link rel="stylesheet" type="text/css" href="<%=path%>/desktop/css/desktop.css" />

    <!-- JS LIB -->
    <script type="text/javascript" src="<%=path%>/dynamic.jsp">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/DBShcma.jsp">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/WebKeys.jsp">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/bitc.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/common.js">
    </script>
    <script language="JavaScript" src="<%=path%>/resources/js/prototype.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-base.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-all-debug.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-lang-zh_CN.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ext-extra.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchPanel.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/MultiSearchPanel.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/RowEditor.js">
    </script>
    <script type="text/javascript" src="<%=path%>/resources/js/CheckColumn.js">
    </script>

    <!-- DESKTOP -->
    <script type="text/javascript" src="<%=path%>/desktop/js/StartMenu.js">
    </script>
    <script type="text/javascript" src="<%=path%>/desktop/js/TaskBar.js">
    </script>
    <script type="text/javascript" src="<%=path%>/desktop/js/Desktop.js">
    </script>
    <script type="text/javascript" src="<%=path%>/desktop/js/App.js">
    </script>
    <script type="text/javascript" src="<%=path%>/desktop/js/Module.js">
    </script>
    <script type="text/javascript" src="<%=path%>/desktop/sample.js">
    </script>
  </head>
  <body scroll="no">

    <div id="x-desktop">
      <a href="#" style="margin: 5px; float: right;">
        <img src="<%=path%>/resources/images/comp_logo_text.png" />
      </a>

      <!-- Shortcuts -->
      <dl id="x-shortcuts">
        <dt id="doc-win-shortcut">
        <a href="#">
          <img src="images/s.gif" />
          <div>
            我的文档
          </div>
        </a>
      </dt>
      <dt id="acc-win-shortcut">
      <a href="#">
        <img src="images/s.gif" />
        <div>
          联系人
        </div>
      </a>
    </dt>
  </dl>
</div>
<!-- TaskBar -->
<div id="ux-taskbar">
  <div id="ux-taskbar-start">
  </div>
  <div id="ux-taskbuttons-panel">
  </div>
  <div class="x-clear">
  </div>
</div>

</body>
</html>
